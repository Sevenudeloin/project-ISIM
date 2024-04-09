#pragma once

#include <list>
#include <tuple>

#include "color.hh"
#include "material.hh"
#include "vector3.hh"

class TerrainTextureParameters
{
public:
    std::list<std::tuple<double, std::shared_ptr<TerrainLayerTexture>>>
        terrain_layers_textures_;

    std::shared_ptr<TerrainLayerTexture>
        above_texture_; // color for height above the highest
                        // normal_height_colors_ (usually snow)

    double cliff_threshold_; // steepness threshold for cliffs in term of
                             // n_y [0, 1]
    std::shared_ptr<TerrainLayerTexture> cliff_texture_; // color for cliffs

    double beach_height_; // height for the beach (relative positive height from
                          // the sea level)
    std::shared_ptr<TerrainLayerTexture> beach_texture_; // color for the beach

    // cliff_threshold is in degrees [0, 90]
    TerrainTextureParameters(
        std::list<std::tuple<double, std::shared_ptr<TerrainLayerTexture>>>
            terrain_layers_textures,
        std::shared_ptr<TerrainLayerTexture> above_texture,
        double cliff_threshold,
        std::shared_ptr<TerrainLayerTexture> cliff_texture, double beach_height,
        std::shared_ptr<TerrainLayerTexture> beach_texture);

    TerrainTextureParameters();

    LocalTexture getTerrainTexture(Point3 p, Vector3 n, double sea_level) const;
};