#include "triangle.hh"

#include <iostream>

#include "utils.hh"

Triangle::Triangle(Point3 v0, Point3 v1, Point3 v2,
                   shared_ptr<TextureMaterial> mat, const PhysObj *parent)
    : PhysObj{ mat }
    , v0_(v0)
    , v1_(v1)
    , v2_(v2)
    , parent_{ parent }
{
    Vector3 v0v1 = v1 - v0;
    Vector3 v0v2 = v2 - v0;
    n_ = Vector3::unit_vector(Vector3::cross(v0v1, v0v2));
}

void Triangle::set_material(shared_ptr<TextureMaterial> mat)
{
    mat_ = mat;
}

void Triangle::translate(const Vector3 &v)
{
    translation_ = translation_ + v;
}

Point3 Triangle::v0() const
{
    if (parent_ != nullptr)
    {
        return parent_->translation_ + v0_;
    }
    return translation_ + v0_;
}

Point3 Triangle::v1() const
{
    if (parent_ != nullptr)
    {
        return parent_->translation_ + v1_;
    }
    return translation_ + v1_;
}

Point3 Triangle::v2() const
{
    if (parent_ != nullptr)
    {
        return parent_->translation_ + v2_;
    }
    return translation_ + v2_;
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
    if (nDotRayDir > 0)
    {
        // Ray pointing away from the plane (backface culling)
        return false;
    }

    double d = -Vector3::dot(n_, v0());
    double t = -(Vector3::dot(n_, ray.origin_) + d) / nDotRayDir;
    if (t < 0)
    {
        return false;
    }

    Point3 p = ray.at(t);

    // Inside-outside test
    Vector3 c;

    // Edge 0
    Vector3 edge0 = v1() - v0();
    Vector3 vp0 = p - v0();
    c = Vector3::cross(edge0, vp0);
    if (Vector3::dot(n_, c) < 0)
    {
        // p is on the right side of the edge 0
        return false;
    }

    // Edge1
    Vector3 edge1 = v2() - v1();
    Vector3 vp1 = p - v1();
    c = Vector3::cross(edge1, vp1);
    if (Vector3::dot(n_, c) < 0)
    {
        // p is on the right side of the edge 1
        return false;
    }

    // Edge2
    Vector3 edge2 = v0() - v2();
    Vector3 vp2 = p - v2();
    c = Vector3::cross(edge2, vp2);
    if (Vector3::dot(n_, c) < 0)
    {
        // p is on the right side of the edge 2
        return false;
    }

    // std::cout << "ray origin: " << ray.origin_ << std::endl;
    // std::cout << "ray direction: " << ray.direction_ << std::endl;
    // std::cout << t << " " << p << std::endl;

    hit_record.t = t;
    hit_record.p = p;
    if (parent_ != nullptr)
    {
        hit_record.n = parent_->get_normal_at(p);
        hit_record.tex = parent_->get_texture_at(p);
    }
    else
    {
        hit_record.n = n_;
        hit_record.tex = get_texture_at(p);
    }

    return true;
}