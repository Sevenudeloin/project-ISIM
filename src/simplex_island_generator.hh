#pragma once

#include "heightmap.hh"

struct SimplexIslandParameters
{
    // Simplex noise generator parameters
    float scale;
    // int octaves = 5 + std::log(scale);
    // float frequency = 0.1f/scale;
    float amplitude;
    float lacunarity;
    float persistence;

    // Heightmap generator parameters
    float offset_x;
    float offset_y;
    float offset_z;

    // Island-shape flattening parameters
    float flatness_amount;
    
    SimplexIslandParameters(float scale, float offset_x, float offset_y, float offset_z, float flatness_amount);
    SimplexIslandParameters(float scale, float amplitude, float lacunarity, float persistence, float offset_x, float offset_y, float offset_z, float flatness_amount);
};

class SimplexIslandGenerator
{
public:
    SimplexIslandGenerator() = default;

    void generateHeightmaps(Heightmap& base_heightmap, Heightmap& upscaled_heightmap, const SimplexIslandParameters& params);
};