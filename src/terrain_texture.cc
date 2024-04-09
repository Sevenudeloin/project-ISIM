#include "terrain_texture.hh"

#include "normal_map_generator.hh"
#include "ppm_parser.hh"
#include "terrain_texture_map_generator.hh"

TerrainTexture::TerrainTexture(std::shared_ptr<Heightmap> height_map,
                               double sea_level, double strength,
                               double xy_scale,
                               const TerrainTextureParameters &params,
                               int quality_factor)
    : height_map_(height_map)
    , sea_level_(sea_level)
    , params_(params)
    , quality_factor_(quality_factor)
{
    normal_map_ = std::make_shared<Image2D>(
        NormalMapGenerator::generateNormalMap(height_map_, strength, xy_scale));

    texture_map_ =
        std::make_shared<Image2D>(height_map_->width_ * quality_factor,
                                  height_map_->height_ * quality_factor);
    texture_properties_map_ =
        std::make_shared<Image2D>(height_map_->width_, height_map_->height_);

    TerrainTextureMapGenerator::generateTerrainTextureMap(
        height_map_, normal_map_, params_, sea_level, texture_map_,
        texture_properties_map_, quality_factor);

    texture_map_->writePPM("../images/texture_map.ppm");
    texture_properties_map_->writePPM("../images/texture_properties_map.ppm");
}

Point3 TerrainTexture::get_uv(const Point3 &p, double quality_factor) const
{
    double x = p.x_;
    double y = p.z_;

    x = Interval(0.0, normal_map_->width_ * quality_factor)
            .clamp(x * normal_map_->width_ * quality_factor);
    y = Interval(0.0, normal_map_->height_ * quality_factor)
            .clamp(y * normal_map_->height_ * quality_factor);

    return Point3(x, p.y_, y);
}

// p is in terrain local coordinates (0, _, 0) to (1, _, 1)
LocalTexture TerrainTexture::get_texture_at(const Point3 &p) const
{
    LocalTexture tex;
    Point3 uv = get_uv(p, quality_factor_);
    Point3 small_uv = get_uv(p);

    tex.color_ = texture_map_->interpolate(uv.z_, uv.x_);
    Color properties =
        texture_properties_map_->interpolate(small_uv.z_, small_uv.x_);

    tex.kd_ = properties.r_;
    tex.ks_ = properties.g_;
    tex.ns_ = properties.b_;
    tex.emission_ = properties.a_;

    return tex;
}

// p is in terrain local coordinates (0, 0, _) to (1, 1, _)
Vector3 TerrainTexture::get_normal_at(const Point3 &p) const
{
    Point3 uv = get_uv(p);
    return normal_map_->getNormal(uv.z_, uv.x_, true);
}