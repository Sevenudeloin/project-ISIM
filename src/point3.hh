#pragma once

#include <ostream>

class Vector3;

class Point3
{
// euclidian geometry point
public:
    double x_ = 0;
    double y_ = 0;
    double z_ = 0;

    Point3() = default; // origin
    Point3(double x, double y, double z); // point from coordinates

    Point3 operator+(const Vector3& vect); // point + vector = point
    Point3 operator-(const Vector3& vect); // point - vector = point
    
    Vector3 operator+(const Point3& point); // point + point = vector
    Vector3 operator-(const Point3& point); // point - point = vector

    bool operator==(const Point3& point) const; 
    bool operator!=(const Point3& point) const;

    friend Point3 operator+(const Point3& point, const Vector3& vect);
    friend Point3 operator-(const Point3& point, const Vector3& vect);

    friend Vector3 operator+(const Point3& point1, const Point3& point2);
    friend Vector3 operator-(const Point3& point1, const Point3& point2);

    friend std::ostream& operator<<(std::ostream& os, const Point3& p); // print point
};

inline std::ostream& operator<<(std::ostream& os, const Point3& p)
{
    os << "(" << p.x_ << ", " << p.y_ << ", " << p.z_ << ")";
    return os;
}