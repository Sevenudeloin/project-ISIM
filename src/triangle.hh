#pragma once

#include "material.hh"
#include "physobj.hh"
#include "ray.hh"
#include "vector3.hh"

using std::shared_ptr;

class Triangle : public PhysObj
{
public:
    Point3 v0_;
    Point3 v1_;
    Point3 v2_;
    Vector3 n_;
    const PhysObj *parent_;

    Triangle(Point3 v0, Point3 v1, Point3 v2, shared_ptr<TextureMaterial> mat,
             const PhysObj *parent = nullptr);

    void set_material(shared_ptr<TextureMaterial> mat);

    void translate(const Vector3 &v) override;

    Point3 v0() const;
    Point3 v1() const;
    Point3 v2() const;

    bool hit(const Ray &ray, HitRecord &hit_record) const override;
};