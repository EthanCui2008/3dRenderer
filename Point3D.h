#ifndef POINT3D_H
#define POINT3D_H

#include <cmath>

#include "CNUMPP_H.h"

struct Vector3D {
    float x;
    float y;
    float z;

    Vector3D() : x(0), y(0), z(0) {}
  
    Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}
};

float dotproduct(Vector3D a, Vector3D b){
    float a_len = pow((pow(a.x,2) + pow(a.y,2) + pow(a.z,2) ),(1/2));
    float b_len = pow((pow(b.x,2) + pow(b.y,2) + pow(b.z,2) ),(1/2));
    float prod = (a.x*b.x+a.y*b.y+a.z*b.z)/(a_len*b_len);
    return std::acos(prod);
}

#endif