#include "material.hh"

#include <cmath>

#include "ppm_parser.hh"

LocalTexture::LocalTexture()
    : color_(Color())
    , kd_(1.0)
    , ks_(0)
    , ns_(0)
    , emission_(0)
{}

LocalTexture::LocalTexture(Color color, double kd, double ks, double ns)
    : color_(color)
    , kd_(kd)
    , ks_(ks)
    , ns_(ns)
    , emission_(0)
{}

LocalTexture::LocalTexture(Color color, double kd, double ks, double ns,
                           double emission)
    : color_(color)
    , kd_(kd)
    , ks_(ks)
    , ns_(ns)
    , emission_(emission)
{}

UniformTexture::UniformTexture(LocalTexture tex)
    : tex_(tex)
{}

LocalTexture UniformTexture::get_texture_at(const Point3 &) const
{
    return tex_;
}

Vector3 UniformTexture::get_normal_at(const Point3 &) const
{
    return Vector3(0, 0, 0);
}

TerrainTexture::TerrainTexture(LocalTexture tex,
                               const std::string &normal_filename,
                               double normal_strength)
    : tex_(tex)
    , normal_map_(Image2D())
    , normal_strength_(normal_strength)
{
    PPMParser parser(normal_filename);

    parser.parse(normal_map_);
}

LocalTexture TerrainTexture::get_texture_at(const Point3 &) const
{
    return tex_;
}

// p is a in terrain local coordinates (0, 0, _) to (1, 1, _)
Vector3 TerrainTexture::get_normal_at(const Point3 &p) const
{
    double x = p.x_;
    double y = p.z_;

    x = Interval(0.0, normal_map_.width_).clamp(x * normal_map_.width_);
    y = Interval(0.0, normal_map_.height_).clamp(y * normal_map_.height_);

    return normal_map_.getNormal(y, x);
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

Vector3 OceanTexture::get_normal_at(const Point3 &p) const
{
    double x = p.x_;
    double y = p.z_;

    // double normal_strength = normal_scale_.y_;

    if (normal_scale_.x_ == 0 || normal_scale_.z_ == 0)
        return Vector3(0, 0, 0);
    x = (x / normal_scale_.x_) - std::floor(x / normal_scale_.x_);
    y = (y / normal_scale_.z_) - std::floor(y / normal_scale_.z_);

    x = Interval(0.0, normal_map_.width_).clamp(x * normal_map_.width_);
    y = Interval(0.0, normal_map_.height_).clamp(y * normal_map_.height_);

    return normal_map_.getNormal(y, x);
}