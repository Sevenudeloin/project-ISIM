#pragma once

#include "material.hh"
#include "terrain.hh"

class OceanTexture : public TextureMaterial
{
public:
    LocalTexture tex_;
    std::shared_ptr<Image2D> normal_map_;
    Vector3 normal_scale_;
    std::shared_ptr<Image2D> wave_map_;
    std::shared_ptr<Image2D> foam_map_;
    std::shared_ptr<Terrain> terrain_;

    OceanTexture(LocalTexture tex, std::shared_ptr<Image2D> normal_map,
                 std::shared_ptr<Terrain> terrain, double sea_level,
                 Vector3 normal_scale = Vector3(1.0, 1.0, 1.0));

    Point3 get_uv(const Point3 &p) const;

    LocalTexture get_texture_at(const Point3 &p) const override;
    Vector3 get_normal_at(const Point3 &p) const override;
};