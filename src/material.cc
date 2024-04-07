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
