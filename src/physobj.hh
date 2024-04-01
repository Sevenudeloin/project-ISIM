#pragma once

#include <memory>

#include "interval.hh"
#include "material.hh"
#include "vector3.hh"

class HitRecord
{
public:
    double t;
    Point3 p;
    Vector3 n;
    LocalTexture tex;
};

class PhysObj
{
protected:
    std::shared_ptr<TextureMaterial> mat_;

public:
    PhysObj()
        : mat_(std::make_shared<UniformTexture>(default_mat))
    {}

    PhysObj(std::shared_ptr<TextureMaterial> mat)
        : mat_(mat)
    {}

    virtual ~PhysObj() = default;

    virtual bool hit(const Vector3 &ray, HitRecord &hit_record) const = 0;

    virtual LocalTexture get_texture_at(const Point3 p) const
    {
        return mat_->get_texture_at(p);
    }
};