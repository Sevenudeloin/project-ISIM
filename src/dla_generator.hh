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

    // static std::uniform_int_distribution<std::mt19937::result_type> dist4_;
    static std::uniform_real_distribution<float> real_dist_1_;
    static std::uniform_real_distribution<float> real_dist_1_zero_centered_;
    static std::uniform_real_distribution<float> real_dist_2pi_;

public:
    float density_threshold_; // grid density required to stop populating the grid
    float graph_center_y_; // y ratio of the graph center (also first node) (0 < y < 1) (will be scaled to the current grid)
    float graph_center_x_; // x ratio of the graph center (also first node) (0 < x < 1) (will be scaled to the current grid)

    DLAGenerator();
    DLAGenerator(float density_threshold);
    DLAGenerator(float density_threshold, int seed); // use fixed seed if need to get reproducible results
    DLAGenerator(float density_threshold, float first_node_y, float first_node_x, int seed);

    std::array<float, 2> getRandom2DPixelCoordinates(int width, int height); // no real need to put it here but needs random engine class attribute
    void populateGraph(int width, Graph& graph);

    void upscaleGraph(Graph& graph);
    Heightmap upscaleBlurryGrid(const Heightmap& low_res_blurry_grid);

    // TODO REMOVE
    void setGraphHeightValues(Graph& graph);
    Heightmap graphToHeightmap(int width, const Graph& graph);

    // Generate high resolution (square) heightmap using DLA algorithm
    Heightmap generateUpscaledHeightmap(int width);

    // We only have to generate the upscaled_heightmap and downsample it to get the base_heightmap
    void generateHeightmaps(Heightmap& base_heightmap, Heightmap& upscaled_heightmap);
};

} // namespace DLA