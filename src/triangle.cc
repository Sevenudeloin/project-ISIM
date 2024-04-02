#include "triangle.hh"

#include "utils.hh"

Triangle::Triangle(Point3 v0, Point3 v1, Point3 v2,
                   shared_ptr<TextureMaterial> mat)
    : PhysObj{ mat }
    , v0_(v0)
    , v1_(v1)
    , v2_(v2)
{
    Vector3 v0v1 = v1 - v0;
    Vector3 v0v2 = v2 - v0;
    n_ = Vector3::unit_vector(Vector3::cross(v0v1, v0v2));
}

void Triangle::set_material(shared_ptr<TextureMaterial> mat)
{
    mat_ = mat;
}

bool Triangle::hit(const Ray &ray, HitRecord &hit_record) const
{
    // check if the ray and triangle plane are parallel
    double nDotRayDir = Vector3::dot(n_, ray.direction_);
    if (Interval(-utils::kEpsilon, +utils::kEpsilon).surrounds(nDotRayDir))
    {
        // Ray perpendicular to the plane
        return false;
    }

    double d = -Vector3::dot(n_, v0_);
    double t = -(Vector3::dot(n_, ray.origin_) + d) / nDotRayDir;
    if (t < 0)
    {
        return false;
    }

    Point3 p = ray.at(t);

    // Inside-outside test
    Vector3 c;

    // Edge 0
    Vector3 edge0 = v1_ - v0_;
    Vector3 vp0 = p - v0_;
    c = Vector3::cross(edge0, vp0);
    if (Vector3::dot(n_, c) < 0)
    {
        // p is on the right side of the edge 0
        return false;
    }

    // Edge1
    Vector3 edge1 = v2_ - v1_;
    Vector3 vp1 = p - v1_;
    c = Vector3::cross(edge1, vp1);
    if (Vector3::dot(n_, c) < 0)
    {
        // p is on the right side of the edge 1
        return false;
    }

    // Edge2
    Vector3 edge2 = v0_ - v2_;
    Vector3 vp2 = p - v2_;
    c = Vector3::cross(edge2, vp2);
    if (Vector3::dot(n_, c) < 0)
    {
        // p is on the right side of the edge 2
        return false;
    }

    hit_record.t = t;
    hit_record.p = p;
    hit_record.n = n_;
    hit_record.tex = get_texture_at(p);
    return true;
}