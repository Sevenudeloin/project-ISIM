#pragma once

#include "heightmap.hh"
#include "image2d.hh"
#include "terrain_texture_parameters.hh"

class TerrainTextureMapGenerator
{
public:
    static void generateTerrainTextureMap(
        std::shared_ptr<Heightmap> height_map,
        std::shared_ptr<Image2D> normal_map,
        const TerrainTextureParameters &params, double sea_level,
        std::shared_ptr<Image2D> texture_map,
        std::shared_ptr<Image2D> texture_properties_map, int quality_factor);
};