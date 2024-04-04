#include "material.hh"

#include <cmath>

#include "ppm_parser.hh"

LocalTexture::LocalTexture()
    : kd_(Color())
    , ks_(0)
    , ns_(0)
{}

LocalTexture::LocalTexture(Color col, double kd, double ks, double ns)
    : kd_(col * kd)
    , ks_(ks)
    , ns_(ns)
{}

UniformTexture::UniformTexture(LocalTexture tex)
    : tex_(tex)
{}

LocalTexture UniformTexture::get_texture_at(const Point3 &) const
{
    return tex_;
}

Vector3 UniformTexture::get_normal_at(double, double) const
{
    return Vector3(0, 0, 0);
}

OceanTexture::OceanTexture(LocalTexture tex, const std::string &normal_filename,
                           Vector3 normal_scale)
    : tex_(tex)
    , normal_map_(Image2D())
    , normal_scale_(normal_scale)
{
    PPMParser parser(normal_filename);

    parser.parse(normal_map_);
}

LocalTexture OceanTexture::get_texture_at(const Point3 &) const
{
    return tex_;
}

Vector3 OceanTexture::get_normal_at(double y, double x) const
{
    double normal_strength = normal_scale_.y_;

    if (normal_scale_.x_ == 0 || normal_scale_.z_ == 0)
        return Vector3(0, 0, 0);
    x = (x / normal_scale_.x_) - std::floor(x / normal_scale_.x_);
    y = (y / normal_scale_.z_) - std::floor(y / normal_scale_.z_);

    x = Interval(0.0, normal_map_.width_).clamp(x * normal_map_.width_);
    y = Interval(0.0, normal_map_.height_).clamp(y * normal_map_.height_);

    Color col = normal_map_.interpolate(y, x);
    auto n = normal_strength * Vector3(col.r_ - 0.5, 0, col.g_ - 0.5);
    return n;
}