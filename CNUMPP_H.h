#ifndef CNUMPP_H
#define CNUMPP_H

#include <iostream>
#include <cmath>

void matmul(float* matrix1, int rows1, int cols1, float* matrix2, int rows2, int cols2, float* result) {
    if (cols1 != rows2) {
        std::cout << "Dumbass" << std::endl;
        return;
    }

    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            result[i * cols2 + j] = 0;
            for (int k = 0; k < cols1; ++k) {
                result[i * cols2 + j] += matrix1[i * cols1 + k] * matrix2[k * cols2 + j];
            }
        }
    }
}
void printmat(float* matrix, int row, int col){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            std::cout << matrix[i * row + j] << " ";
        }
        std::cout << std::endl;
    }
}
void copy(float* source, float* destination, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            destination[i * cols + j] = source[i * cols + j];
        }
    }
}
void xrotate(float* matrix1, int rows1, int xdeg, float* result){
    float xRad = xdeg * M_PI / 180.0;
    float sinX = std::sin(xRad); float cosX = std::cos(xRad);

    float xRotation[4][4] = {
        {1, 0, 0, 0},
        {0, cosX, sinX, 0},
        {0, -sinX, cosX, 0},
        {0, 0, 0, 1}
    };
    matmul(matrix1, rows1, 4, *xRotation, 4, 4, result);
}

void yrotate(float* matrix1, int rows1, int ydeg, float* result){
    float yRad = ydeg * M_PI / 180.0;
    float sinY = std::sin(yRad); float cosY = std::cos(yRad);

    float yRotation[4][4] = {
        {cosY, 0, -sinY, 0},
        {0, 1, 0, 0},
        {sinY, 0, cosY, 0},
        {0, 0, 0, 1}
    };

    matmul(matrix1, rows1, 4, *yRotation, 4, 4, result);
}

void zrotate(float* matrix1, int rows1, int zdeg, float* result){
    float zRad = zdeg * M_PI / 180.0;
    float sinZ = std::sin(zRad); float cosZ = std::cos(zRad);

    float zRotation[4][4] = {
        {cosZ, sinZ, 0, 0},
        {-sinZ, cosZ, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    matmul(matrix1, rows1, 4, *zRotation, 4, 4, result);
}

void move(float* matrix1, int rows1, float x, float y, float z, float* result){
    float move[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {x, y, z, 1}
    };
    matmul(matrix1, rows1, 4, *move, 4, 4, result);
}

void move(float* matrix1, int rows1, float x, float y, float z, float* result){
    float move[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {x, y, z, 1}
    };
    matmul(matrix1, rows1, 4, *move, 4, 4, result);
}


#endif /* CNUMPP_H i hate being called smart it makes me feel even worse about how washed up and wasted my life is :sob: */