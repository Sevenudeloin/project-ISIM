#include "material.hh"
#include "normal_map_generator.hh"
#include "ppm_parser.hh"
#include "terrain_texture_map_generator.hh"
#include "terrain_texture_parameters.hh"

TerrainTexture::TerrainTexture(LocalTexture tex,
                               std::shared_ptr<Heightmap> height_map,
                               double sea_level, double strength,
                               double xy_scale)
    : tex_(tex)
    , height_map_(height_map)
    , sea_level_(sea_level)
{
    normal_map_ = std::make_shared<Image2D>(
        NormalMapGenerator::generateNormalMap(height_map_, strength, xy_scale));
    texture_map_ = std::make_shared<Image2D>(
        TerrainTextureMapGenerator::generateTerrainTextureMap(
            height_map_, normal_map_, TerrainTextureParameters(), sea_level));
}

// p is in terrain local coordinates (0, _, 0) to (1, _, 1)
LocalTexture TerrainTexture::get_texture_at(const Point3 &p) const
{
    LocalTexture tex = tex_;

    double x = p.x_;
    double y = p.z_;

    x = Interval(0.0, texture_map_->width_).clamp(x * texture_map_->width_);
    y = Interval(0.0, texture_map_->height_).clamp(y * texture_map_->height_);

    tex.color_ = texture_map_->interpolate(y, x);
    return tex;
}

// p is in terrain local coordinates (0, 0, _) to (1, 1, _)
Vector3 TerrainTexture::get_normal_at(const Point3 &p) const
{
    double x = p.x_;
    double y = p.z_;

    x = Interval(0.0, normal_map_->width_).clamp(x * normal_map_->width_);
    y = Interval(0.0, normal_map_->height_).clamp(y * normal_map_->height_);

    return normal_map_->getNormal(y, x, true);
}