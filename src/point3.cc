#include "point3.hh"

#include "vector3.hh"

Point3::Point3(double x, double y, double z)
    : x_(x), y_(y), z_(z)
{}

Point3 Point3::operator+(const Vector3& vect)
{
    return Point3(x_ + vect.x_, y_ + vect.y_, z_ + vect.z_);
}

Point3 Point3::operator-(const Vector3& vect)
{
    return Point3(x_ - vect.x_, y_ - vect.y_, z_ - vect.z_);
}

Vector3 Point3::operator+(const Point3& point)
{
    return Vector3(*this, x_ + point.x_, y_ + point.y_, z_ + point.z_);
}

Vector3 Point3::operator-(const Point3& point)
{
    return Vector3(*this, x_ - point.x_, y_ - point.y_, z_ - point.z_);
}

bool Point3::operator==(const Point3& point) const
{
    return x_ == point.x_ && y_ == point.y_ && z_ == point.z_;
}

bool Point3::operator!=(const Point3& point) const
{
    return !(*this == point);
}

Point3 operator+(const Point3& point, const Vector3& vect)
{
    return Point3(point.x_ + vect.x_, point.y_ + vect.y_, point.z_ + vect.z_);
}

Point3 operator-(const Point3& point, const Vector3& vect)
{
    return Point3(point.x_ - vect.x_, point.y_ - vect.y_, point.z_ - vect.z_);
}

Vector3 operator+(const Point3& point1, const Point3& point2)
{
    return Vector3(point1, point1.x_ + point2.x_, point1.y_ + point2.y_, point1.z_ + point2.z_);
}

Vector3 operator-(const Point3& point1, const Point3& point2)
{
    return Vector3(point1, point1.x_ - point2.x_, point1.y_ - point2.y_, point1.z_ - point2.z_);
}