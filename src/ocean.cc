#include "ocean.hh"

#include "utils.hh"

Ocean::Ocean(Point3 center, float r, shared_ptr<TextureMaterial> mat)
    : PhysObj(mat)
    , center_(center)
    , r_(r)
    , n_(0, 1, 0)
{}

void Ocean::translate(const Vector3 &v)
{
    center_ = center_ + v;
}

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

    double d = -Vector3::dot(n_, center_);
    double t = -(Vector3::dot(n_, ray.origin_) + d) / nDotRayDir;
    if (t < 0)
    {
        return false;
    }

    Point3 p = ray.at(t);

    if ((p - center_).length() > r_)
    {
        return false;
    }

    hit_record.t = t;
    hit_record.p = p;
    Point3 local_p = ((p - center_) / r_ + Point3(1.0, 0.0, 1.0)) / 2.0;
    hit_record.n =
        Vector3::unit_vector(n_ + get_normal_at_local(local_p.x_, local_p.z_));
    hit_record.tex = get_texture_at(p);
    return true;
}