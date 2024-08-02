#ifndef POINT3D_H
#define POINT3D_H

#include <cmath>

#include "BITMAP_H.h"
#include "CNUMPP_H.h"
#include "STLREADER_H.h"
#include "Point3D.h"

struct Vector3D {
    float x;
    float y;
    float z;

    Vector3D() : x(0), y(0), z(0) {}
  
    Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

    friend std::ostream& operator<<(std::ostream& os, const Vector3D& point) {
        os << "<" << point.x << ", " << point.y << ", " << point.z << ">";
        return os;
    }
};

float dotproduct(const Vector3D& a, const Vector3D& b) {
    float dot = a.x * b.x + a.y * b.y + a.z * b.z;
    float a_len = std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    float b_len = std::sqrt(b.x * b.x + b.y * b.y + b.z * b.z);

    float prod = dot / (a_len * b_len);
    prod = std::clamp(prod, -1.0f, 1.0f);

    return std::acos(prod);
}

#endif