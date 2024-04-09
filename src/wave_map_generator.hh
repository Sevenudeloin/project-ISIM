#pragma once

#include <memory>

#include "heightmap.hh"
#include "image2d.hh"
#include "wave_map_parameters.hh"

class WaveMapGenerator
{
public:
    static Image2D
    generateShoreWaveMap(std::shared_ptr<Heightmap> terrain_height_map,
                         const WaveMapParameters &params,
                         double sea_level = 0.0);

    static Image2D
    generateDeepOceanWaveMap(std::shared_ptr<Image2D> ocean_normal_map,
                             const WaveMapParameters &params);

    static Heightmap
    generateAboveWaterMap(std::shared_ptr<Heightmap> terrain_height_map,
                          double sea_level);

    static Heightmap generateDistToShoreMap(const Heightmap &above_water_map);

    static Image2D distanceMapToWaveMap(const Heightmap &distances,
                                        const WaveMapParameters &params);
};