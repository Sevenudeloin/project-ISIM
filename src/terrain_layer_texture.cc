#include <cmath>
#include <iostream>

#include "material.hh"

TerrainLayerTexture::TerrainLayerTexture(LocalTexture tex,
                                         std::shared_ptr<Image2D> texture_map,
                                         Vector3 scale)
    : tex_(tex)
    , texture_map_(texture_map)
    , scale_(scale)
{}

Point3 TerrainLayerTexture::get_uv(const Point3 &p) const
{
    double x = p.x_;
    double y = p.z_;

    if (scale_.x_ == 0 || scale_.z_ == 0)
        return Vector3(0, 0, 0);
    x = (x / scale_.x_) - std::floor(x / scale_.x_);
    y = (y / scale_.z_) - std::floor(y / scale_.z_);

    x = Interval(0.0, texture_map_->width_).clamp(x * texture_map_->width_);
    y = Interval(0.0, texture_map_->height_).clamp(y * texture_map_->height_);

    return Point3(x, p.y_, y);
}

// p is in terrain local coordinates (0, 0, 0) to (1, 1, 1)
LocalTexture TerrainLayerTexture::get_texture_at(const Point3 &p) const
{
    LocalTexture tex = tex_;
    Point3 uv = get_uv(p);
    tex.color_ = texture_map_->getPixel(uv.z_, uv.x_);
    return tex;
}

// Should not be called
Vector3 TerrainLayerTexture::get_normal_at(const Point3 &) const
{
    return Vector3(0, 0, 0);
}

const TerrainLayerTexture TerrainLayerTexture::grass_texture(
    LocalTexture(Color(), 0.99, 0.01, 0.3),
    std::make_shared<Image2D>("../images/textures/grass_texture.ppm"),
    Vector3(0.1, 0.0, 0.1));

const TerrainLayerTexture TerrainLayerTexture::cliff_texture(
    LocalTexture(Color(), 0.9, 0.1, 1.0),
    std::make_shared<Image2D>("../images/textures/cliff_texture.ppm"),
    Vector3(0.2, 0.0, 0.2));

const TerrainLayerTexture TerrainLayerTexture::beach_texture(
    LocalTexture(Color(), 0.95, 0.05, 0.5),
    std::make_shared<Image2D>("../images/textures/beach_texture.ppm"),
    Vector3(0.15, 0.0, 0.15));

const TerrainLayerTexture TerrainLayerTexture::snow_texture(
    LocalTexture(Color(), 0.8, 0.2, 0.7),
    std::make_shared<Image2D>("../images/textures/snow_texture.ppm"),
    Vector3(0.2, 0.0, 0.2));