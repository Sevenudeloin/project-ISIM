#include "ray.hh"

#include "vector3.hh"

Ray::Ray()
    : origin_(Point3())
    , direction_(Vector3())
{}

Ray::Ray(double x, double y, double z)
    : origin_(Point3())
    , direction_(Vector3(x, y, z))
{}

Ray::Ray(const Point3& origin, double x, double y, double z)
    : origin_(origin)
    , direction_(Vector3(x, y, z))
{}

Ray::Ray(double xorig, double yorig, double zorig, const Vector3& direction)
    : origin_(Point3(xorig, yorig, zorig))
    , direction_(direction)
{}

Ray::Ray(const Point3& origin, const Vector3& direction)
    : origin_(origin)
    , direction_(direction)
{}

Ray::Ray(double xorig, double yorig, double zorig, double x, double y, double z)
    : origin_(Point3(xorig, yorig, zorig))
    , direction_(Vector3(x, y, z))
{}

Point3 Ray::at(double t) const
{
    return origin_ + t * direction_;
}
