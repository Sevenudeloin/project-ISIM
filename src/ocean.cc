#include "ocean.hh"

#include <iostream>

#include "utils.hh"

Ocean::Ocean(double height, shared_ptr<TextureMaterial> mat)
    : PhysObj(mat)
    , height_(height)
    , n_(0, 1, 0)
{}

bool Ocean::hit(const Ray &ray, HitRecord &hit_record) const
{
    double nDotRayDir = Vector3::dot(n_, ray.direction_);
    if (Interval(-utils::kEpsilon, +utils::kEpsilon).surrounds(nDotRayDir))
    {
        // Ray perpendicular to the plane
        return false;
    }
    if (nDotRayDir > 0)
    {
        // Ray pointing away from the plane (backface culling)
        return false;
    }

    double d = -Vector3::dot(n_, Point3(0, height_, 0));
    double t = -(Vector3::dot(n_, ray.origin_) + d) / nDotRayDir;
    if (t < 0)
    {
        return false;
    }

    Point3 p = ray.at(t);

    hit_record.t = t;
    hit_record.p = p;
    hit_record.n = get_normal_at(p);
    hit_record.tex = get_texture_at(p);
    return true;
}

void Ocean::translate(const Vector3 &v)
{
    height_ += v.y_;
}