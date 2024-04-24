#pragma once

#include <map>

#include "heightmap.hh"

struct Edge
{
    std::array<int, 2> nodes;
    float height; // keep this ?
};

// Experiment with generator hyperparameters to get different results
class DLAGenerator // Diffusion Limited Aggregation
{
public:
    using Graph = std::map<int, std::vector<Edge>>; // ints are identifiers

    // TODO add constructors
    // default (find default values for parameters)
    // with parameters as arguments

    void populateGrid(Heightmap& grid, Graph& graph);

    // Generate high resolution (square) heightmap using DLA algorithm
    Heightmap generateUpscaledHeightmap(int width);

    // Here we will have to generate the upscaled one only, and downsample it to get the base_heightmap
    void generateHeightmaps(Heightmap& base_heightmap, Heightmap& upscaled_heightmap);

    // TODO add attributes here
    float density_threshold_; // grid density required to stop the grid population
};
