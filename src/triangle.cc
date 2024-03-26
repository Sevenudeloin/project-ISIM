#include "triangle.hh"

Triangle::Triangle(const Point3& a, const Point3& b, const Point3& c)
    : a_(a)
    , b_(b)
    , c_(c)
{}

Vector3 Triangle::getNormal(const Point3& point) // TODO fix if normal pointing in wrong direction
{
    return Vector3(point, Vector3::unit_vector(Vector3::cross(b_ - a_, c_ - a_)));
}