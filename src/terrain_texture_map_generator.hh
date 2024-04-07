#pragma once

#include "heightmap.hh"
#include "image2d.hh"
#include "terrain_texture_parameters.hh"

class TerrainTextureMapGenerator
{
public:
    static Image2D
    generateTerrainTextureMap(std::shared_ptr<Heightmap> height_map,
                              std::shared_ptr<Image2D> normal_map,
                              const TerrainTextureParameters &params,
                              double sea_level = 0.0);
};