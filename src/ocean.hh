#pragma once

#include "physobj.hh"

using std::shared_ptr;

class Ocean : public PhysObj
{
public:
    Point3 center_;
    float r_;
    Vector3 n_;

    Ocean(Point3 center, float r, shared_ptr<TextureMaterial> mat);

    void translate(const Vector3 &v) override;

    bool hit(const Ray &ray, HitRecord &hit_record) const override;
};