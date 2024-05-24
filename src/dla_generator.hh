#pragma once

#include <random>

#include "dla_graph.hh"
#include "heightmap.hh"

namespace DLA {

// TODO: Experiment with generator hyperparameters to get different results
class DLAGenerator // Diffusion Limited Aggregation
{
private:
    std::random_device rd_;
    std::mt19937 rng_;

    static std::uniform_int_distribution<std::mt19937::result_type> dist4_;
    // static std::uniform_int_distribution<std::mt19937::result_type> dist5_;

public:
    // TODO add more attributes here
    float density_threshold_; // grid density required to stop populating the grid

    DLAGenerator();
    DLAGenerator(float density_threshold);
    DLAGenerator(float density_threshold, int seed); // use fixed seed if need to get reproducible results

    std::array<int, 2> getRandom2DPixelCoordinates(int width, int height); // no real need to put it here but needs random engine class attribute
    void populateGrid(Heightmap& grid, Graph& graph); // FIXME: maybe give the graph as shared_ptr

    Heightmap upscaleCrispGrid(const Heightmap& low_res_crisp_grid, Graph& graph);
    Heightmap upscaleBlurryGrid(const Heightmap& low_res_blurry_grid);

    // Generate high resolution (square) heightmap using DLA algorithm
    Heightmap generateUpscaledHeightmap(int width);

    // We only have to generate the upscaled_heightmap and downsample it to get the base_heightmap
    void generateHeightmaps(Heightmap& base_heightmap, Heightmap& upscaled_heightmap);
};

} // namespace DLA