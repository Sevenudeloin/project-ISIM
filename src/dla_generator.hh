#pragma once

#include "heightmap.hh"

class DLAGenerator {
    void populateGrid(Heightmap& grid, float density_threshold);

    // Here we will have to generate the upscaled one only, and downsample it to get the base_heightmap
    void generateHeightmaps(Heightmap& base_heightmap, Heightmap& upscaled_heightmap);
};