#pragma once

#include <memory>

#include "material.hh"
#include "ray.hh"
#include "vector3.hh"

struct HitRecord
{
    double t;
    Point3 p;
    Vector3 n;
    LocalTexture tex;
};

class PhysObj
{
public:
    std::shared_ptr<TextureMaterial> mat_;

    PhysObj();

    PhysObj(std::shared_ptr<TextureMaterial> mat);

    virtual ~PhysObj() = default;

    virtual bool hit(const Ray &ray, HitRecord &hit_record) const = 0;

    virtual void translate(const Vector3 &v) = 0;

    virtual LocalTexture get_texture_at(const Point3 &p) const;
    virtual Vector3 get_normal_at(double y, double x) const;
};