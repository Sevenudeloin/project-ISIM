#include "vector3.hh"

#include <cmath>

#include "point3.hh"
#include "utils.hh"

Vector3::Vector3()
    : origin_(Point3())
    , x_(0)
    , y_(0)
    , z_(0)
{}

Vector3::Vector3(double x, double y, double z)
    : origin_(Point3())
    , x_(x)
    , y_(y)
    , z_(z)
{}

Vector3::Vector3(const Point3& origin, double x, double y, double z)
    : origin_(origin)
    , x_(x)
    , y_(y)
    , z_(z)
{}

Vector3::Vector3(const Point3& origin, const Point3& point)
    : origin_(origin)
    , x_(point.x_ - origin.x_)
    , y_(point.y_ - origin.y_)
    , z_(point.z_ - origin.z_)
{}

Vector3::Vector3(const Point3& origin, const Vector3& direction)
    : origin_(origin)
    , x_(direction.x_)
    , y_(direction.y_)
    , z_(direction.z_)
{}

Vector3::Vector3(double xorig, double yorig, double zorig, double x, double y, double z)
    : origin_(Point3(xorig, yorig, zorig))
    , x_(x)
    , y_(y)
    , z_(z)
{}


Vector3 Vector3::operator+(const Vector3& vect)
{
    return Vector3(origin_, x_ + vect.x_, y_ + vect.y_, z_ + vect.z_); // origin is not updated
}

Vector3 Vector3::operator-(const Vector3& vect)
{
    return Vector3(origin_, x_ - vect.x_, y_ - vect.y_, z_ - vect.z_); // origin is not updated
}

Vector3 Vector3::operator-()
{
    return Vector3(origin_, -x_, -y_, -z_); // origin is not updated
}

Vector3 Vector3::operator*(const double& scalar)
{
    return Vector3(origin_, x_ * scalar, y_ * scalar, z_ * scalar); // origin is not updated (obviously)
}

Vector3 Vector3::operator/(const double& scalar)
{
    return Vector3(origin_, x_ / scalar, y_ / scalar, z_ / scalar); // origin is not updated (obviously)
}

double Vector3::dot(const Vector3& vect) const
{
    return x_ * vect.x_ + y_ * vect.y_ + z_ * vect.z_;
}

double Vector3::dot(const Vector3& vect1, const Vector3& vect2)
{
    return vect1.x_ * vect2.x_ + vect1.y_ * vect2.y_ + vect1.z_ * vect2.z_;
}

Vector3 Vector3::cross(const Vector3& vect) const
{
    return Vector3(origin_, y_ * vect.z_ - z_ * vect.y_, z_ * vect.x_ - x_ * vect.z_, x_ * vect.y_ - y_ * vect.x_);
}

Vector3 Vector3::cross(const Vector3& vect1, const Vector3& vect2)
{
    return Vector3(vect1.origin_, vect1.y_ * vect2.z_ - vect1.z_ * vect2.y_, vect1.z_ * vect2.x_ - vect1.x_ * vect2.z_, vect1.x_ * vect2.y_ - vect1.y_ * vect2.x_);
}

double Vector3::length() const
{
    return sqrt(length_squared());
}

double Vector3::length_squared() const {
    return x_ * x_ + y_ * y_ + z_ * z_;
}

Vector3 Vector3::unit_vector(Vector3 vect)
{
    return vect / vect.length();
}

Point3 Vector3::at(double t) const
{
    return origin_ + (*this * t);
}

Vector3 Vector3::random_vector()
{
    return Vector3(utils::random_double(), utils::random_double(), utils::random_double());
}

Vector3 Vector3::random_vector(double min, double max)
{
    return Vector3(utils::random_double(min, max), utils::random_double(min, max), utils::random_double(min, max));
}

Vector3 Vector3::random_in_unit_sphere()
{
    while (true) {
        auto p = random_vector(-1, 1);
        if (p.length_squared() < 1)
            return p;
    }
}

Vector3 Vector3::random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

Vector3 Vector3::random_on_hemisphere(const Vector3& normal)
{
    Vector3 on_unit_sphere = random_unit_vector();
    if (Vector3::dot(on_unit_sphere, normal) > 0.0) // In same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

Vector3 operator-(const Vector3& vect1, const Vector3& vect2)
{
    return Vector3(vect1.origin_, vect1.x_ - vect2.x_, vect1.y_ - vect2.y_, vect1.z_ - vect2.z_); // origin is the one of vect1
}

Vector3 operator-(const Vector3& vect)
{
    return Vector3(vect.origin_, -vect.x_, -vect.y_, -vect.z_); // origin is not updated
}

Vector3 operator*(double scalar, const Vector3& vect)
{
    return Vector3(vect.origin_, scalar * vect.x_, scalar * vect.y_, scalar * vect.z_); // origin is not updated
}

Vector3 operator*(const Vector3& vect, double scalar)
{
    return Vector3(vect.origin_, scalar * vect.x_, scalar * vect.y_, scalar * vect.z_); // origin is not updated
}