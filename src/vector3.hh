#pragma once

#include <ostream>

class Vector3
{
    // mathematical representation of a point in 3D space (vector from origin)
public:
    double x_;
    double y_;
    double z_;

    Vector3() = default; // origin or null vector
    Vector3(double x, double y,
            double z); // from origin to specific coordinates

    Vector3 operator+(const Vector3 &vect);
    Vector3 operator-(const Vector3 &vect);
    friend Vector3 operator+(const Vector3 &vect1, const Vector3 &vect2);
    friend Vector3 operator-(const Vector3 &vect1, const Vector3 &vect2);

    Vector3 operator-();
    friend Vector3 operator-(const Vector3 &vect);

    Vector3 operator*(const double &scalar);
    friend Vector3 operator*(double scalar, const Vector3 &vect);
    friend Vector3 operator*(const Vector3 &vect, double scalar);

    Vector3 operator/(const double &scalar);

    bool operator==(const Vector3 &vect) const;
    bool operator!=(const Vector3 &vect) const;

    double dot(const Vector3 &vect) const; // dot product on instance
    static double dot(const Vector3 &vect1,
                      const Vector3 &vect2); // static dot product
    Vector3 cross(const Vector3 &vect) const; // cross product on instance
    static Vector3 cross(const Vector3 &vect1,
                         const Vector3 &vect2); // static cross product

    static Vector3 reflect(const Vector3 &v, const Vector3 &n);
    static Vector3 refract(const Vector3 &u, const Vector3 &n,
                           double etai_over_etat);

    double length() const; // length of the vector
    double
    length_squared() const; // length of the vector squared (eucledian norm)
    static Vector3 unit_vector(Vector3 vect); // static normalization of vector
                                              // (convert to unit vector)

    static Vector3 random_vector(); // random vector
    static Vector3
    random_vector(double min,
                  double max); // random vector with min and max values

    // FIXME keep these ?
    static Vector3 random_in_unit_sphere();
    static Vector3 random_unit_vector();
    static Vector3 random_on_hemisphere(const Vector3 &normal);

    friend std::ostream &operator<<(std::ostream &os,
                                    const Vector3 &vect); // print

    static Vector3 spherical_to_cartesian(double rho, double lati,
                                          double longi);
    static Vector3 cartesian_to_cylindric(const Vector3 &vect);
};

using Point3 = Vector3;

inline std::ostream &operator<<(std::ostream &os, const Vector3 &vect)
{
    os << "(" << vect.x_ << ", " << vect.y_ << ", " << vect.z_ << ")";
    return os;
}