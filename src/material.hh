#pragma once

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

    LocalTexture();

    LocalTexture(Color color, double kd, double ks, double ns, double emission);
    LocalTexture(Color color, double kd, double ks, double ns);
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

class TerrainTexture : public TextureMaterial
{
public:
    LocalTexture tex_;
    std::shared_ptr<Heightmap> height_map_;
    std::shared_ptr<Image2D> normal_map_;
    std::shared_ptr<Image2D> texture_map_;
    double sea_level_;

    TerrainTexture(LocalTexture tex, std::shared_ptr<Heightmap> height_map,
                   double sea_level, double strength, double xy_scale);

    LocalTexture get_texture_at(const Point3 &p) const override;
    Vector3 get_normal_at(const Point3 &p) const override;
};

class OceanTexture : public TextureMaterial
{
public:
    LocalTexture tex_;
    Image2D normal_map_;
    Vector3 normal_scale_;

    OceanTexture(LocalTexture tex, const std::string &normal_filename,
                 Vector3 normal_scale = Vector3(1.0, 1.0, 1.0));

    LocalTexture get_texture_at(const Point3 &p) const override;
    Vector3 get_normal_at(const Point3 &p) const override;
};