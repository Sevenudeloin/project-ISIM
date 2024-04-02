#pragma once

#include <ostream>

#include "vector3.hh"

class Ray
{
// ray (has an origin and a direction)
public:
    Point3 origin_;
    Vector3 direction_;

    Ray(); // null ray
    Ray(double x, double y, double z); // ray from origin (0, 0, 0)
    Ray(const Point3& origin, double x, double y, double z);
    Ray(double xorig, double yorig, double zorig, const Vector3& direction);
    Ray(const Point3& origin, const Vector3& direction);
    Ray(double xorig, double yorig, double zorig, double x, double y, double z);

    Point3 at(double t) const; // point at t on the ray

    friend std::ostream& operator<<(std::ostream& os, const Vector3& vect); // print ray
};

inline std::ostream& operator<<(std::ostream& os, const Ray& ray)
{
    os << "origin: " << ray.origin_ << ", direction: " << ray.direction_;
    return os;
}