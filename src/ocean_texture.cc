#include "ocean_texture.hh"

#include <cmath>

#include "ppm_parser.hh"
#include "terrain_texture.hh"
#include "wave_map_generator.hh"

OceanTexture::OceanTexture(LocalTexture tex,
                           std::shared_ptr<Image2D> normal_map,
                           std::shared_ptr<Terrain> terrain, double sea_level,
                           Vector3 normal_scale)
    : tex_(tex)
    , normal_map_(normal_map)
    , normal_scale_(normal_scale)
    , terrain_(terrain)
{
    auto params = WaveMapParameters();
    wave_map_ = std::make_shared<Image2D>(
        WaveMapGenerator::generateDeepOceanWaveMap(normal_map_, params));
    foam_map_ =
        std::make_shared<Image2D>(WaveMapGenerator::generateShoreWaveMap(
            dynamic_cast<TerrainTexture *>(terrain->mat_.get())->height_map_,
            params, sea_level));
}

Point3 OceanTexture::get_uv(const Point3 &p) const
{
    double x = p.x_;
    double y = p.z_;

    if (normal_scale_.x_ == 0 || normal_scale_.z_ == 0)
        return Vector3(0, 0, 0);
    x = (x / normal_scale_.x_) - std::floor(x / normal_scale_.x_);
    y = (y / normal_scale_.z_) - std::floor(y / normal_scale_.z_);

    x = Interval(0.0, normal_map_->width_).clamp(x * normal_map_->width_);
    y = Interval(0.0, normal_map_->height_).clamp(y * normal_map_->height_);

    return Point3(x, p.y_, y);
}

LocalTexture OceanTexture::get_texture_at(const Point3 &p) const
{
    LocalTexture tex = tex_;
    Point3 local_coords = get_uv(p);
    Color wave_color = wave_map_->interpolate(local_coords.z_, local_coords.x_);

    tex.color_ = tex.color_ + wave_color;

    auto test = p - terrain_->translation_;
    test.x_ = test.x_ / (terrain_->width_ * terrain_->xy_scale_);
    test.z_ = test.z_ / (terrain_->height_ * terrain_->xy_scale_);
    if (test.x_ > 0.0 && test.x_ < 1.0 && test.z_ > 0.0 && test.z_ < 1.0)
    {
        Color foam_color = foam_map_->interpolate(test.z_ * foam_map_->height_,
                                                  test.x_ * foam_map_->width_);
        if (foam_color.a_ > 0.0)
        {
            tex.color_.a_ = foam_color.a_;
        }
    }

    return tex;
}

Vector3 OceanTexture::get_normal_at(const Point3 &p) const
{
    Point3 local_coords = get_uv(p);
    return normal_map_->getNormal(local_coords.z_, local_coords.x_);
}