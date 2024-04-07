#include "clouds_plan.hh"

#include <cmath>

#include "utils.hh"

CloudsPlan::CloudsPlan(std::shared_ptr<Image2D> clouds_mask,
                       double clouds_height, double clouds_scale,
                       double clouds_max_opacity)
    : clouds_mask_(clouds_mask)
    , clouds_height_(clouds_height)
    , clouds_scale_(clouds_scale)
    , clouds_max_opacity_(clouds_max_opacity)
    , n_(Vector3(0, -1, 0))
{}

void CloudsPlan::translate(const Vector3 &v)
{
    clouds_height_ += v.y_;
}

bool CloudsPlan::hit(const Ray &ray, HitRecord &hit_record) const
{
    double nDotRayDir = Vector3::dot(n_, ray.direction_);
    if (Interval(-utils::kEpsilon, +utils::kEpsilon).surrounds(nDotRayDir))
    {
        // Ray perpendicular to the plane
        return false;
    }

    double d = -Vector3::dot(n_, Point3(0, clouds_height_, 0));
    double t = -(Vector3::dot(n_, ray.origin_) + d) / nDotRayDir;
    if (t < 0)
    {
        return false;
    }

    Point3 p = ray.at(t);

    hit_record.t = t;
    hit_record.p = p;
    hit_record.n = n_;
    hit_record.tex = get_texture_at(p);
    return true;
}

LocalTexture CloudsPlan::get_texture_at(const Point3 &p) const
{
    LocalTexture tex;
    double x = p.x_;
    double y = p.z_;

    if (clouds_scale_ == 0)
        return LocalTexture();
    x = (x / clouds_scale_) - std::floor(x / clouds_scale_);
    y = (y / clouds_scale_) - std::floor(y / clouds_scale_);

    x = Interval(0.0, clouds_mask_->width_).clamp(x * clouds_mask_->width_);
    y = Interval(0.0, clouds_mask_->height_).clamp(y * clouds_mask_->height_);

    tex.color_ = clouds_mask_->interpolate(y, x, true);
    return tex;
}