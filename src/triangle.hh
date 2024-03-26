#pragma once

#include "point3.hh"
#include "vector3.hh"

class Triangle
{
public:
    Point3 a_;
    Point3 b_;
    Point3 c_;

    Triangle(const Point3& a, const Point3& b, const Point3& c);

    Vector3 getNormal(const Point3& point);
};