#include "terrain_texture_map_generator.hh"

Image2D TerrainTextureMapGenerator::generateTerrainTextureMap(
    std::shared_ptr<Heightmap> height_map, std::shared_ptr<Image2D> normal_map,
    const TerrainTextureParameters &params, double sea_level)
{
    Image2D texture_map(height_map->width_, height_map->height_);

    for (int i = 0; i < height_map->width_; i++)
    {
        for (int j = 0; j < height_map->height_; j++)
        {
            double height = height_map->at(i, j);
            Vector3 n = normal_map->getNormal(i, j, true);

            Color pixel_color = params.getTerrainColor(height, n, sea_level);

            texture_map.setPixel(i, j, pixel_color);
        }
    }

    return texture_map;
}