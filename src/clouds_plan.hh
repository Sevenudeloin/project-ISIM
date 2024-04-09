#pragma once

#include "image2d.hh"
#include "physobj.hh"

class CloudsPlan : public PhysObj
{
public:
    std::shared_ptr<Image2D> clouds_mask_;
    double clouds_height_;
    double clouds_scale_;
    double clouds_max_opacity_;
    Vector3 n_;

    CloudsPlan(std::shared_ptr<Image2D> clouds_mask, double clouds_height,
               double clouds_scale, double clouds_max_opacity);

    bool hit(const Ray &ray, HitRecord &hit_record) const override;

    void translate(const Vector3 &v) override;

    LocalTexture get_texture_at(const Point3 &p) const override;
};