#include "simplex_island_generator.hh"

#include "simplex_noise.hh"

SimplexIslandParameters::SimplexIslandParameters(float scale, float offset_x, float offset_y, float offset_z, float flatness_amount)
    : scale(scale)
    , amplitude(0.5f)
    , lacunarity(1.99f)
    , persistence(0.5f)
    , offset_x(offset_x)
    , offset_y(offset_y)
    , offset_z(offset_z)
    , flatness_amount(flatness_amount)
{}

SimplexIslandParameters::SimplexIslandParameters(float scale, float amplitude, float lacunarity, float persistence, float offset_x, float offset_y, float offset_z, float flatness_amount)
    : scale(scale)
    , amplitude(amplitude)
    , lacunarity(lacunarity)
    , persistence(persistence)
    , offset_x(offset_x)
    , offset_y(offset_y)
    , offset_z(offset_z)
    , flatness_amount(flatness_amount)
{}

void SimplexIslandGenerator::generateHeightmaps(Heightmap& base_heightmap, Heightmap& upscaled_heightmap, const SimplexIslandParameters& params)
{
    // upscaling based on width only (we assume we are working with square heightmaps for now)
    float upscaling = static_cast<float>(upscaled_heightmap.width_) / base_heightmap.width_;

    auto base_simplexNoiseGenerator = SimplexNoiseGenerator(params.scale, params.amplitude, params.lacunarity, params.persistence);
    base_heightmap = base_simplexNoiseGenerator.generateHeightmap(base_heightmap.width_, base_heightmap.height_, params.scale, params.offset_x, params.offset_y, params.offset_z);

    // base_heightmap = base_heightmap.flattenSides(base_heightmap.width_ / params.flatness_amount);
    base_heightmap = base_heightmap.flattenSides(base_heightmap.width_ / 18.f);

    auto upscaled_simplexNoiseGenerator = SimplexNoiseGenerator(params.scale * upscaling, params.amplitude, params.lacunarity, params.persistence);
    upscaled_heightmap = upscaled_simplexNoiseGenerator.generateHeightmap(upscaled_heightmap.width_, upscaled_heightmap.height_, params.scale * upscaling, params.offset_x, params.offset_y, params.offset_z);

    // upscaled_heightmap = upscaled_heightmap.flattenSides(upscaled_heightmap.width_ / params.flatness_amount);
    upscaled_heightmap = upscaled_heightmap.flattenSides(upscaled_heightmap.width_ / 18.f);
}