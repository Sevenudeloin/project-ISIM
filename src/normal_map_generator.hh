#pragma once

#include "heightmap.hh"
#include "image2d.hh"

class NormalMapGenerator
{
public:
    static Image2D generateNormalMap(std::shared_ptr<Heightmap> height_map,
                                     double strength, double xy_scale);
};