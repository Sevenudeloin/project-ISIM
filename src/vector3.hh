#pragma once

#include <ostream>

#include "point3.hh"

class Vector3
{
// euclidian geometry vector
// has an origin and a direction
public:
    Point3 origin_;
    double x_;
    double y_;
    double z_;

    Vector3(); // null vector
    Vector3(double x, double y, double z); // vector from origin (0, 0, 0)
    Vector3(const Point3& origin, double x, double y, double z); // vector from given point and direction
    Vector3(const Point3& origin, const Point3& point); // vector from starting point and end point
    Vector3(const Point3& origin, const Vector3& direction); // vector from starting point and vector (represents only a direction)
    Vector3(double xorig, double yorig, double zorig, double x, double y, double z); // vector from starting point and direction

    Vector3 operator+(const Vector3& vect);
    Vector3 operator-(const Vector3& vect);
    Vector3 operator-();
    Vector3 operator*(const double& scalar);
    Vector3 operator/(const double& scalar);

    double dot(const Vector3& vect) const; // dot product on instance
    static double dot(const Vector3& vect1, const Vector3& vect2); // static dot product
    Vector3 cross(const Vector3& vect) const; // cross product on instance
    static Vector3 cross(const Vector3& vect1, const Vector3& vect2); // static cross product

    double length() const; // length of the vector
    double length_squared() const; // length of the vector squared (eucledian norm) 
    static Vector3 unit_vector(Vector3 vect); // static normalization of vector (convert to unit vector)

    Point3 at(double t) const; // point at t on the vector

    static Vector3 random_vector(); // random vector
    static Vector3 random_vector(double min, double max); // random vector with min and max values

    // FIXME keep these ?
    static Vector3 random_in_unit_sphere();
    static Vector3 random_unit_vector();
    static Vector3 random_on_hemisphere(const Vector3& normal);

    friend Vector3 operator-(const Vector3& vect1, const Vector3& vect2);
    friend Vector3 operator-(const Vector3& vect);
    friend Vector3 operator*(double scalar, const Vector3& vect);
    friend Vector3 operator*(const Vector3& vect, double scalar);

    friend std::ostream& operator<<(std::ostream& os, const Vector3& vect); // print vector
};

inline std::ostream& operator<<(std::ostream& os, const Vector3& vect)
{
    os << "(origin: " << vect.origin_ << ", dir: (" << vect.x_ << ", " << vect.y_ << ", " << vect.z_ << "))";
    return os;
}