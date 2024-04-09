#pragma once

#include "absorption_volume.hh"
#include "color.hh"
#include "heightmap.hh"
#include "image2d.hh"
#include "vector3.hh"

struct LocalTexture
{
    Color color_;
    double kd_;
    double ks_;
    double ns_;
    double emission_;
    std::shared_ptr<AbsorptionVolume> absorption_;

    LocalTexture();

    LocalTexture(Color color, double kd, double ks, double ns,
                 double emission = 0.0,
                 std::shared_ptr<AbsorptionVolume> absorption = nullptr);
};

class TextureMaterial
{
public:
    virtual ~TextureMaterial() = default;

    virtual LocalTexture get_texture_at(const Point3 &p) const = 0;
    virtual Vector3 get_normal_at(const Point3 &p) const = 0;
};

class UniformTexture : public TextureMaterial
{
private:
    LocalTexture tex_;

public:
    UniformTexture(LocalTexture tex);

    LocalTexture get_texture_at(const Point3 &p) const override;
    Vector3 get_normal_at(const Point3 &p) const override;

    const static UniformTexture default_mat;
};

const static UniformTexture default_mat(LocalTexture(Color(0.5, 0.5, 0.5), 0.8,
                                                     0.1, 0.5));

class TerrainLayerTexture : public TextureMaterial
{
public:
    LocalTexture tex_;
    std::shared_ptr<Image2D> texture_map_;
    Vector3 scale_;

    TerrainLayerTexture(LocalTexture tex, std::shared_ptr<Image2D> texture_map,
                        Vector3 scale = Vector3(1.0, 1.0, 1.0));

    Point3 get_uv(const Point3 &p) const;

    LocalTexture get_texture_at(const Point3 &p) const override;
    Vector3 get_normal_at(const Point3 &p) const override;

    const static TerrainLayerTexture grass_texture;
    const static TerrainLayerTexture cliff_texture;
    const static TerrainLayerTexture beach_texture;
    const static TerrainLayerTexture snow_texture;
};

class OceanTexture : public TextureMaterial
{
public:
    LocalTexture tex_;
    std::shared_ptr<Image2D> normal_map_;
    Vector3 normal_scale_;
    std::shared_ptr<Image2D> wave_map_;

    OceanTexture(LocalTexture tex, std::shared_ptr<Image2D> normal_map,
                 Vector3 normal_scale = Vector3(1.0, 1.0, 1.0));

    Point3 get_uv(const Point3 &p) const;

    LocalTexture get_texture_at(const Point3 &p) const override;
    Vector3 get_normal_at(const Point3 &p) const override;
};