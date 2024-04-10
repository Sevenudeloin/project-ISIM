#include "vector3.hh"

#include <cmath>

#include "utils.hh"

Vector3::Vector3(double x, double y, double z)
    : x_(x)
    , y_(y)
    , z_(z)
{}

Vector3 Vector3::operator+(const Vector3 &vect)
{
    return Vector3(x_ + vect.x_, y_ + vect.y_, z_ + vect.z_);
}

Vector3 Vector3::operator-(const Vector3 &vect)
{
    return Vector3(x_ - vect.x_, y_ - vect.y_, z_ - vect.z_);
}

Vector3 operator+(const Vector3 &vect1, const Vector3 &vect2)
{
    return Vector3(vect1.x_ + vect2.x_, vect1.y_ + vect2.y_,
                   vect1.z_ + vect2.z_);
}

Vector3 operator-(const Vector3 &vect1, const Vector3 &vect2)
{
    return Vector3(vect1.x_ - vect2.x_, vect1.y_ - vect2.y_,
                   vect1.z_ - vect2.z_);
}

Vector3 Vector3::operator-()
{
    return Vector3(-x_, -y_, -z_);
}

Vector3 operator-(const Vector3 &vect)
{
    return Vector3(-vect.x_, -vect.y_, -vect.z_);
}

Vector3 Vector3::operator*(const double &scalar)
{
    return Vector3(x_ * scalar, y_ * scalar, z_ * scalar);
}

Vector3 operator*(double scalar, const Vector3 &vect)
{
    return Vector3(vect.x_ * scalar, vect.y_ * scalar, vect.z_ * scalar);
}

Vector3 operator*(const Vector3 &vect, double scalar)
{
    return Vector3(vect.x_ * scalar, vect.y_ * scalar, vect.z_ * scalar);
}

Vector3 Vector3::operator/(const double &scalar)
{
    return *this * (1 / scalar);
}

bool Vector3::operator==(const Vector3 &vect) const
{
    return x_ == vect.x_ && y_ == vect.y_ && z_ == vect.z_;
}

bool Vector3::operator!=(const Vector3 &vect) const
{
    return !(*this == vect);
}

double Vector3::dot(const Vector3 &vect) const
{
    return x_ * vect.x_ + y_ * vect.y_ + z_ * vect.z_;
}

double Vector3::dot(const Vector3 &vect1, const Vector3 &vect2)
{
    return vect1.x_ * vect2.x_ + vect1.y_ * vect2.y_ + vect1.z_ * vect2.z_;
}

Vector3 Vector3::cross(const Vector3 &vect) const
{
    return Vector3(y_ * vect.z_ - z_ * vect.y_, z_ * vect.x_ - x_ * vect.z_,
                   x_ * vect.y_ - y_ * vect.x_);
}

Vector3 Vector3::cross(const Vector3 &vect1, const Vector3 &vect2)
{
    return Vector3(vect1.y_ * vect2.z_ - vect1.z_ * vect2.y_,
                   vect1.z_ * vect2.x_ - vect1.x_ * vect2.z_,
                   vect1.x_ * vect2.y_ - vect1.y_ * vect2.x_);
}

Vector3 Vector3::reflect(const Vector3 &v, const Vector3 &n)
{
    return v - 2 * dot(v, n) * n;
}

double Vector3::length() const
{
    return sqrt(length_squared());
}

double Vector3::length_squared() const
{
    return x_ * x_ + y_ * y_ + z_ * z_;
}

Vector3 Vector3::unit_vector(Vector3 vect)
{
    return vect / vect.length();
}

Vector3 Vector3::random_vector()
{
    return Vector3(utils::random_double(), utils::random_double(),
                   utils::random_double());
}

Vector3 Vector3::random_vector(double min, double max)
{
    return Vector3(utils::random_double(min, max),
                   utils::random_double(min, max),
                   utils::random_double(min, max));
}

Vector3 Vector3::random_in_unit_sphere()
{
    while (true)
    {
        auto p = random_vector(-1, 1);
        if (p.length_squared() < 1)
            return p;
    }
}

Vector3 Vector3::random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

Vector3 Vector3::random_on_hemisphere(const Vector3 &normal)
{
    Vector3 on_unit_sphere = random_unit_vector();
    if (Vector3::dot(on_unit_sphere, normal)
        > 0.0) // In same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

Vector3 Vector3::spherical_to_cartesian(double rho, double lati, double longi)
{
    double x = rho * std::sin(lati) * std::cos(longi);
    double y = rho * std::cos(lati);
    double z = rho * std::sin(lati) * std::sin(longi);

    return Vector3(x, y, z);
}

Vector3 Vector3::cartesian_to_cylindric(const Vector3 &vect)
{
    double rho = std::sqrt(vect.x_ * vect.x_ + vect.z_ * vect.z_);
    double theta = 0.0;
    if (vect.x_ == 0)
    {
        if (vect.z_ > 0)
        {
            theta = utils::pi / 2;
        }
        else
        {
            theta = -utils::pi / 2;
        }
    }
    else
    {
        theta = std::atan(vect.x_ / vect.z_);
    }
    double z = vect.y_;

    return Vector3(rho, theta, z);
}
