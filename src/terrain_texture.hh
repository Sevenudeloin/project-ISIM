#include "terrain.hh"
#include "terrain_texture_parameters.hh"

class TerrainTexture : public TextureMaterial
{
public:
    std::shared_ptr<Heightmap> height_map_;
    std::shared_ptr<Image2D> normal_map_;

    std::shared_ptr<Image2D> texture_map_; // color
    std::shared_ptr<Image2D> texture_properties_map_; // kd, ks, ns, emission

    double sea_level_;
    TerrainTextureParameters params_;
    int quality_factor_;

    TerrainTexture(std::shared_ptr<Heightmap> height_map, double sea_level,
                   double strength, double xy_scale,
                   const TerrainTextureParameters &params, int quality_factor);

    Point3 get_uv(const Point3 &p, double quality_factor = 1.0) const;

    LocalTexture get_texture_at(const Point3 &p) const override;
    Vector3 get_normal_at(const Point3 &p) const override;
};