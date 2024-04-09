#pragma once

#include "physobj.hh"
#include "vector3.hh"

class TerrainOceanicPlan : public PhysObj
{
public:
    double height_;
    Vector3 n_;

    TerrainOceanicPlan(double height)
    {}

    bool hit(const Ray &ray, HitRecord &hit_record) const override;

    void translate(const Vector3 &v) override;

    LocalTexture get_texture_at(const Point3 &p) const override;
    Vector3 get_normal_at(const Point3 &p) const override;
}