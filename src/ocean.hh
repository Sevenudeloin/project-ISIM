#pragma once

#include "physobj.hh"

using std::shared_ptr;

class Ocean : public PhysObj
{
public:
    double height_;
    Vector3 n_;

    Ocean(double height, shared_ptr<TextureMaterial> mat);

    void translate(const Vector3 &v) override;

    bool hit(const Ray &ray, HitRecord &hit_record) const override;
};