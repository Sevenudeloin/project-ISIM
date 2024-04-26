#pragma once

#include <map>

#include "heightmap.hh"

struct Node
{
    int label_; // start from 0
    int y_;
    int x_;
    float height_;
};

struct Graph
{
    using AdjacencyList = std::vector<std::vector<int>>; // adjacency list representation of the graph (by label, position in the list is the label of the node)
    using NodesList = std::vector<Node>; // list of nodes in the graph (ideally, the position of the node in the list should be the label of the node)
    // using NodesList = std::vector<std::shared_ptr<Node>>; // TODO use this instead ?

    AdjacencyList adjacency_list_;
    NodesList nodes_list_;
};

// TODO: Experiment with generator hyperparameters to get different results
class DLAGenerator // Diffusion Limited Aggregation
{
public:
    // TODO add attributes here
    float density_threshold_; // grid density required to stop populating the grid

    // TODO add constructors
    // default (find default values for parameters)
    // with parameters as arguments

    void populateGrid(Heightmap& grid, Graph& graph);

    // Generate high resolution (square) heightmap using DLA algorithm
    Heightmap generateUpscaledHeightmap(int width);

    // Here we will have to generate the upscaled one only, and downsample it to get the base_heightmap
    void generateHeightmaps(Heightmap& base_heightmap, Heightmap& upscaled_heightmap);

};
