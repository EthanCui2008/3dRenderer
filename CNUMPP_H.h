#ifndef CNUMPP_H
#define CNUMPP_H

#include <iostream>
#include <cmath>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "Point3D.h"

//matrix multiplication function
inline void matmul(float* matrix1, int rows1, int cols1, float* matrix2, int rows2, int cols2, float* result) {
    if (cols1 != rows2) {
        std::cout << "Matrix dimension mismatch" << std::endl;
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

//copies one 2d array to another
inline void copy(float* source, float* destination, int rows, int cols) {
    for (int i = 0; i < rows * cols; ++i) {
        destination[i] = source[i];
    }
}

//directly modifies 2d array via matmul with second 2d array
inline void matmulModify(float* original, int rows_o, int cols_o, float* modifier, int rows_m, int cols_m) {
    std::vector<float> original_copy(rows_o * cols_o);
    copy(original, original_copy.data(), rows_o, cols_o);
    matmul(original_copy.data(), rows_o, cols_o, modifier, rows_m, cols_m, original);
}

inline void vectorRotationX(Vector3D& v, float deg) {
    float theta = deg * (float)(M_PI / 180.0);
    float cosTheta = std::cos(theta);
    float sinTheta = std::sin(theta);

    float originalY = v.y;
    v.y = originalY * cosTheta - v.z * sinTheta;
    v.z = originalY * sinTheta + v.z * cosTheta;
}

inline void vectorRotationY(Vector3D& v, float deg) {
    float theta = deg * (float)(M_PI / 180.0);
    float cosTheta = std::cos(theta);
    float sinTheta = std::sin(theta);

    float originalX = v.x;
    v.x = originalX * cosTheta + v.z * sinTheta;
    v.z = -originalX * sinTheta + v.z * cosTheta;
}

inline void vectorRotationZ(Vector3D& v, float deg) {
    float theta = deg * (float)(M_PI / 180.0);
    float cosTheta = std::cos(theta);
    float sinTheta = std::sin(theta);

    float originalX = v.x;
    v.x = originalX * cosTheta - v.y * sinTheta;
    v.y = originalX * sinTheta + v.y * cosTheta;
}

inline void facetRotationX(float* matrix1, int rows1, int xdeg, Vector3D& v) {
    vectorRotationX(v, (float)xdeg);
    float xRad = xdeg * (float)(M_PI / 180.0);
    float sinX = std::sin(xRad); float cosX = std::cos(xRad);
    float xRotation[4][4] = {
        {1, 0, 0, 0},
        {0, cosX, sinX, 0},
        {0, -sinX, cosX, 0},
        {0, 0, 0, 1}
    };
    matmulModify(matrix1, rows1, 4, *xRotation, 4, 4);
}

inline void facetRotationY(float* matrix1, int rows1, int ydeg, Vector3D& v) {
    vectorRotationY(v, (float)ydeg);
    float yRad = ydeg * (float)(M_PI / 180.0);
    float sinY = std::sin(yRad); float cosY = std::cos(yRad);

    float yRotation[4][4] = {
        {cosY, 0, -sinY, 0},
        {0, 1, 0, 0},
        {sinY, 0, cosY, 0},
        {0, 0, 0, 1}
    };
    matmulModify(matrix1, rows1, 4, *yRotation, 4, 4);
}

inline void facetRotationZ(float* matrix1, int rows1, int zdeg, Vector3D& v) {
    vectorRotationZ(v, (float)zdeg);
    float zRad = zdeg * (float)(M_PI / 180.0);
    float sinZ = std::sin(zRad); float cosZ = std::cos(zRad);

    float zRotation[4][4] = {
        {cosZ, sinZ, 0, 0},
        {-sinZ, cosZ, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    matmulModify(matrix1, rows1, 4, *zRotation, 4, 4);
}

inline void moveMatrix(float* matrix1, int rows1, float x, float y, float z) {
    float move[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {x, y, z, 1}
    };
    matmulModify(matrix1, rows1, 4, *move, 4, 4);
}

inline void scaleMatrix(float* matrix1, int rows1, float x, float y, float z) {
    float scale[4][4] = {
        {x, 0, 0, 0},
        {0, y, 0, 0},
        {0, 0, z, 0},
        {0, 0, 0, 1}
    };
    matmulModify(matrix1, rows1, 4, *scale, 4, 4);
}

inline void viewVolumeTransform(float* matrix1, int rows1, Vector3D& bottom_near_left_point, Vector3D& top_far_right_point) {
    float left = bottom_near_left_point.y;
    float bottom = bottom_near_left_point.z;
    float near_val = bottom_near_left_point.x;

    float right = top_far_right_point.y;
    float top = top_far_right_point.z;
    float far_val = top_far_right_point.x;

    float view[4][4] = {
        {2.0f / (right - left), 0, 0, 0},
        {0, 2.0f / (top - bottom), 0, 0},
        {0, 0, 2.0f / (far_val - near_val), 0},
        {-(right + left) / (right - left), -(bottom + top) / (bottom - top), -near_val / (far_val - near_val), 1}
    };

    matmulModify(matrix1, rows1, 4, *view, 4, 4);
}

inline void perspectiveTransform(float* matrix1, int rows1, float near_val, float far_val) {
    float project[4][4] = {
        {near_val, 0, 0, 0},
        {0, near_val, 0, 0},
        {0, 0, (far_val + near_val), 1},
        {0, 0, -1 * far_val * near_val, 0}
    };
    matmulModify(matrix1, rows1, 4, *project, 4, 4);
}

#endif
