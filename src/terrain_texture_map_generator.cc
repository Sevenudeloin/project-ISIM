#include "terrain_texture_map_generator.hh"

void TerrainTextureMapGenerator::generateTerrainTextureMap(
    std::shared_ptr<Heightmap> height_map, std::shared_ptr<Image2D> normal_map,
    const TerrainTextureParameters &params, double sea_level,
    std::shared_ptr<Image2D> texture_map,
    std::shared_ptr<Image2D> texture_properties_map, int quality_factor)
{
    for (int i = 0; i < texture_map->height_; i++)
    {
        for (int j = 0; j < texture_map->width_; j++)
        {
            int small_i = i / quality_factor;
            int small_j = j / quality_factor;
            double height = height_map->at(small_i, small_j);
            Vector3 n = normal_map->getNormal(small_i, small_j, true);

            LocalTexture pixel_texture = params.getTerrainTexture(
                Point3(j / static_cast<double>(texture_map->height_), height,
                       i / static_cast<double>(texture_map->width_)),
                n,
                sea_level); // FIXME with real world coordinates

            texture_map->setPixel(i, j, pixel_texture.color_);
            texture_properties_map->setPixel(
                small_i, small_j,
                Color(pixel_texture.kd_, pixel_texture.ks_, pixel_texture.ns_,
                      pixel_texture.emission_));
        }
    }
}