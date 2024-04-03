#pragma once

#include <cstddef>

#include "heightmap.hh"

namespace generators {
    // use simplex noise and fBm to generate a 2D heightmap with default fBm parameters
    Heightmap generateSimplexNoise2D(int width, int height, size_t octaves);
    // use simplex noise and fBm to generate a 2D heightmap
    Heightmap generateSimplexNoise2D(int width, int height, size_t octaves, float frequency, float amplitude, float lacunarity, float persistence);
}