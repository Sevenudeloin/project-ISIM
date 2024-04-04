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

Vector3 UniformTexture::get_normal_at_local(float, float) const
{
    return Vector3(0, 0, 0);
}

Vector3 UniformTexture::get_normal_at_global(const Point3 &) const
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

Vector3 OceanTexture::get_normal_at_local(float y, float x) const
{
    float normal_strength = normal_scale_.y_;
    x = (std::fmod(x, normal_scale_.x_) / normal_scale_.x_);
    y = (std::fmod(y, normal_scale_.z_) / normal_scale_.z_);

    int img_y =
        Interval(0, normal_map_.height_ - 1).clamp(y * normal_map_.height_);
    int img_x =
        Interval(0, normal_map_.width_ - 1).clamp(x * normal_map_.width_);

    Color col = normal_map_.getPixel(img_y, img_x);
    return normal_strength * Vector3(col.r_, 0, col.g_);
}

Vector3 OceanTexture::get_normal_at_global(const Point3 &) const
{
    return Vector3(0.0, 0.0, 0.0);
}