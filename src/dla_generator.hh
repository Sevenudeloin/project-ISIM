#pragma once

#include <memory>
#include <random>

#include "heightmap.hh"

namespace DLA {

struct Node
{
    int label_; // start from 1
    int y_;
    int x_;
    float height_;
};

struct Graph
{
    using AdjacencyList = std::vector<std::vector<int>>; // adjacency list representation of the graph (by label, position in the list is the label of the node)
    using NodesList = std::vector<std::shared_ptr<Node>>; // list of nodes in the graph (position of the node in the vector should be the label of the node)

    AdjacencyList adjacency_list_;
    NodesList nodes_list_;

    Graph() {
        adjacency_list_.push_back(std::vector<int>()); // add a dummy node at the beginning
        nodes_list_.push_back(nullptr); // add a dummy node at the beginning
    };
};

// TODO: Experiment with generator hyperparameters to get different results
class DLAGenerator // Diffusion Limited Aggregation
{
private:
    std::random_device rd_;
    std::mt19937 rng_;
    std::uniform_int_distribution<std::mt19937::result_type> dist4_{1, 4};

public:
    // TODO add more attributes here
    float density_threshold_; // grid density required to stop populating the grid

    DLAGenerator();
    DLAGenerator(float density_threshold);
    DLAGenerator(float density_threshold, int seed); // use fixed seed if need to get reproducible results

    std::array<int, 2> getRandom2DPixelCoordinates(int width, int height); // no real need to put it here but needs random engine class attribute
    void populateGrid(Heightmap& grid, Graph& graph); // FIXME: maybe give the graph as shared_ptr

    // Generate high resolution (square) heightmap using DLA algorithm
    Heightmap generateUpscaledHeightmap(int width);

    // Here we will have to generate the upscaled one only, and downsample it to get the base_heightmap
    void generateHeightmaps(Heightmap& base_heightmap, Heightmap& upscaled_heightmap);
};

}