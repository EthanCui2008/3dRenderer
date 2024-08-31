#ifndef CNUMPP_H
#define CNUMPP_H

#include <iostream>
#include <cmath>

#include "BITMAP_H.h"
#include "CNUMPP_H.h"
#include "STLREADER_H.h"
#include "Point3D.h"


//matrix multiplication function, inputs: 2 2d arrays, and corresponding row and column integers, along with an output array
void matmul(float* matrix1, int rows1, int cols1, float* matrix2, int rows2, int cols2, float* result) {
    if (cols1 != rows2) {
        std::cout << "Dumbass" << std::endl;
        return;
    }

    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i * cols2 + j] = 0;
            for (int k = 0; k < cols1; k++) {
                result[i * cols2 + j] += matrix1[i * cols1 + k] * matrix2[k * cols2 + j];
            }
        }
    }
}

//copies one 2d array to an other given same row and int size, inputs: 2 2d arrays, correspond row and column sizes
void copy(float* source, float* destination, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            destination[i * cols + j] = source[i * cols + j];
        }
    }
}

//directly modifies 2d array via matmul with second 2d array
void matmulModify(float* original, int rows_o, int cols_o, float* modifier, int rows_m, int cols_m) {
    float original_copy[rows_o][cols_o];
    copy(original, *original_copy, rows_o, cols_o);
    matmul(*original_copy, rows_o, cols_o, modifier, rows_m, cols_m, original);
}

//rotates a vector by deg degrees around the x axis, inputs: vector, float degree
void vectorRotationX(Vector3D& v, float deg) {
    float theta = deg * (M_PI / 180.0f);
    float cosTheta = std::cos(theta);
    float sinTheta = std::sin(theta);

    float originalY = v.y;
    v.y = originalY * cosTheta - v.z * sinTheta;
    v.z = originalY * sinTheta + v.z * cosTheta;
}

//rotates a vector by deg degrees around the y axis, inputs: vector, float degree
void vectorRotationY(Vector3D& v, float deg) {
    float theta = deg * (M_PI / 180.0f);
    float cosTheta = std::cos(theta);
    float sinTheta = std::sin(theta);

    float originalX = v.x;
    v.x = originalX * cosTheta + v.z * sinTheta;
    v.z = -originalX * sinTheta + v.z * cosTheta;
}

//rotates a vector by deg degrees around the z axis, inputs: vector, float degree
void vectorRotationZ(Vector3D& v, float deg) {
    float theta = deg * (M_PI / 180.0f);
    float cosTheta = std::cos(theta);
    float sinTheta = std::sin(theta);

    float originalX = v.x;
    v.x = originalX * cosTheta - v.y * sinTheta;
    v.y = originalX * sinTheta + v.y * cosTheta;
}

void facetRotationX(float* matrix1, int rows1, int xdeg, Vector3D& v){
    vectorRotationX(v, xdeg);
    float xRad = xdeg * M_PI / 180.0;
    float sinX = std::sin(xRad); float cosX = std::cos(xRad);
    float xRotation[4][4] = {
        {1, 0, 0, 0},
        {0, cosX, sinX, 0},
        {0, -sinX, cosX, 0},
        {0, 0, 0, 1}
    };
    matmulModify(matrix1, rows1, 4, *xRotation, 4, 4);
}

void facetRotationY(float* matrix1, int rows1, int ydeg, Vector3D& v){
    vectorRotationY(v, ydeg);
    float yRad = ydeg * M_PI / 180.0;
    float sinY = std::sin(yRad); float cosY = std::cos(yRad);

    float yRotation[4][4] = {
        {cosY, 0, -sinY, 0},
        {0, 1, 0, 0},
        {sinY, 0, cosY, 0},
        {0, 0, 0, 1}
    };
    matmulModify(matrix1, rows1, 4, *yRotation, 4, 4);
}

void facetRotationZ(float* matrix1, int rows1, int zdeg, Vector3D& v){
    vectorRotationZ(v, zdeg);
    float zRad = zdeg * M_PI / 180.0;
    float sinZ = std::sin(zRad); float cosZ = std::cos(zRad);

    float zRotation[4][4] = {
        {cosZ, sinZ, 0, 0},
        {-sinZ, cosZ, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    matmulModify(matrix1, rows1, 4, *zRotation, 4, 4);
}

//moves matrix of values by corresponding x, y, z, inputs are as such too
void moveMatrix(float* matrix1, int rows1, float x, float y, float z){
    float move[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {x, y, z, 1}
    };
    matmulModify(matrix1, rows1, 4, *move, 4, 4);
}

//scales matrix of values by corresponding x, y, z
void scaleMatrix(float* matrix1, int rows1, float x, float y, float z){
    float scale[4][4] = {
        {x, 0, 0, 0},
        {0, y, 0, 0},
        {0, 0, z, 0},
        {0, 0, 0, 1}
    };
    matmulModify(matrix1, rows1, 4, *scale, 4, 4);
}

//view volume transform, takes all points within bounding box defined by r [right]
void viewVolumeTransform(float* matrix1, int rows1, Vector3D& bottom_near_left_point, Vector3D& top_far_right_point){
    
    int left = bottom_near_left_point.y;
    int bottom = bottom_near_left_point.z;
    int near = bottom_near_left_point.x;

    int right =  top_far_right_point.y;
    int top =  top_far_right_point.z;
    int far = top_far_right_point.x;

    float view[4][4] = {
        {2/(right-left), 0, 0, 0},
        {0, 2/(top-bottom), 0, 0},
        {0, 0, 2/(far-near), 0},
        {-1*(right+left)/(right-left), -1*(bottom+top)/(bottom-top), -1*near/(far-near), 1}
    }; 

    matmulModify(matrix1, rows1, 4, *view, 4, 4);
}

void perspectiveTransform(float* matrix1, int rows1, float near, float far){
    near = -20;
    far = 20;
    float project[4][4] = {
        {near, 0, 0, 0},
        {0, near, 0, 0},
        {0, 0, (far+near), 1},
        {0, 0, -1*far*near, 0}
    }; //not actually needed because we don't keep track of object stacking
    matmulModify(matrix1, rows1, 4, *project, 4, 4);
}

#endif