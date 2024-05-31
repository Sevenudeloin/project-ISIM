#include "dla_generator.hh"

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <memory>
#include <set>
#include <vector>

#include "dla_graph.hh"
#include "heightmap.hh"
#include "utils.hh"

#include "image2d.hh"

// Implementation of DLA Algorithm (intuition from https://youtu.be/gsJHzBTPG0Y?si=jipP7Z0xBVCW3Ip6):
// algorithm bounded to CPU (but we dont care)

namespace DLA {

// std::uniform_int_distribution<std::mt19937::result_type> DLAGenerator::dist4_(1, 4);
std::uniform_real_distribution<float> DLAGenerator::real_dist1_zero_centered_(-1.0f, 1.0f);

DLAGenerator::DLAGenerator()
    : rng_(rd_())
    , density_threshold_(0.5f)
    , graph_center_y_(0.5f)
    , graph_center_x_(0.5f)
{}

DLAGenerator::DLAGenerator(float density_threshold)
    : rng_(rd_())
    , density_threshold_(density_threshold)
    , graph_center_y_(0.5f)
    , graph_center_x_(0.5f)
{}

DLAGenerator::DLAGenerator(float density_threshold, int seed)
    : rng_(seed)
    , density_threshold_(density_threshold)
    , graph_center_y_(0.5f)
    , graph_center_x_(0.5f)
{}

DLAGenerator::DLAGenerator(float density_threshold, float graph_center_y, float graph_center_x, int seed)
    : rng_(seed)
    , density_threshold_(density_threshold)
{
    if (graph_center_y < 0.0f || graph_center_y >= 1.0f || graph_center_x < 0.0f || graph_center_x >= 1.0f) {
        throw std::runtime_error("DLAGenerator: DLAGenerator: Graph center must be in the range [0.0, 1.0)");
    }

    graph_center_y_ = graph_center_y;
    graph_center_x_ = graph_center_x;
}

/**
 * @brief Format for coordinates: { y, x }
 *
 * @param[in] width   grid width
 * @param[in] height  grid height
 *
 * @return random floating-point coordinates for a pixel in a 2D continuous grid
 */
std::array<float, 2> DLAGenerator::getRandom2DPixelCoordinates(int width, int height) {
    std::uniform_real_distribution<float> dist_height(0.0f, height);
    std::uniform_real_distribution<float> dist_width(0.0f, width);

    float height_pos = -1.0f;
    while (height_pos < 0 || height_pos >= height)
        height_pos = dist_height(rng_);

    float width_pos = -1.0f;
    while (width_pos < 0 || width_pos >= width)
        width_pos = dist_width(rng_);

    return { height_pos, width_pos };
}

// float euclidianDistance(float y1, float x1, float y2, float x2) {
//     return std::sqrt(std::pow(y1 - y2, 2) + std::pow(x1 - x2, 2));
// }

/**
 * @brief Add nodes to the graph until a certain density threshold is reached. Nodes are spawned randomly and
 * move in a random direction continously on the grid until they are close enough to another node.
 * An edge is created between this node and the node that it stuck to. If there are multiple pixels next to it,
 * the edge is created with the node closest to the center of the graph.
 *
 * @param[in] width       square grid width
 * @param[in, out] graph  DLA graph
 */
void DLAGenerator::populateGraph(int width, Graph& graph) {
    if (graph.nodes_list_.size() <= 1) { // dummy node
        throw std::runtime_error("DLAGenerator: populateGrid: Need at least one node on the graph to populate it");
    }

    float density = static_cast<float>(graph.nodes_list_.size() - 1) / (width * width);

    while (density < this->density_threshold_) {
        std::array<float, 2> pixel_coords = getRandom2DPixelCoordinates(width, width);
        float y = pixel_coords[0];
        float x = pixel_coords[1];

        // check if there is already a graph node too close to this position (small radius, here 0.1)
        while (graph.getNodesAround(y, x, 0.1).size() > 0) {
            pixel_coords = getRandom2DPixelCoordinates(width, width);
            y = pixel_coords[0];
            x = pixel_coords[1];
        }

        while (true) {
            // check if the pixel is next to another pixel (1 radius)
            std::vector<int> labels_around = graph.getNodesAround(y, x, 1.f);

            if (labels_around.size() > 0) {
                // add it to the graph and continue the main loop
                // (if multiple pixels next to it, add edge to the one closest to the center of the grid)

                int node_label = graph.nodes_list_.size();
                graph.nodes_list_.push_back(std::make_shared<Node>(node_label, y, x, -1.0f));
                graph.adjacency_list_.push_back({});

                // std::cout << "Node: " << node_label << " at (" << y << ", " << x << ")" << std::endl;

                int label_closest_to_center = -1;
                float min_distance_to_center = std::numeric_limits<float>::max();

                for (int label : labels_around) {
                    float node_y = graph.nodes_list_[label]->y_;
                    float node_x = graph.nodes_list_[label]->x_;
                    
                    float distance_to_center = utils::euclidianDistance(node_y, node_x, graph_center_y_ * width, graph_center_x_ * width);

                    if (distance_to_center < min_distance_to_center) {
                        label_closest_to_center = label;
                        min_distance_to_center = distance_to_center;
                    }
                }

                graph.adjacency_list_[node_label].push_back(label_closest_to_center);
                graph.adjacency_list_[label_closest_to_center].push_back(node_label);

                break;
            }

            // move the pixel randomly on the continuous grid
            
            float new_y = y + real_dist1_zero_centered_(rng_); // [-1, 1)
            float new_x = x + real_dist1_zero_centered_(rng_); // [-1, 1)

            while (new_y < 0 || new_y >= width || new_x < 0 || new_x >= width || graph.getNodesAround(new_y, new_x, 0.1).size() > 0) {
                new_y = y + real_dist1_zero_centered_(rng_);
                new_x = x + real_dist1_zero_centered_(rng_);
            }

            y = new_y;
            x = new_x;
        }

        density = static_cast<float>(graph.nodes_list_.size() - 1) / (width * width);
    }
}

bool edgeAlreadyProcessed(const std::vector<std::array<int, 2>>& processed_edges, int node1_label, int node2_label) {
    for (const auto& edge : processed_edges) {
        if ((edge[0] == node1_label && edge[1] == node2_label) || (edge[0] == node2_label && edge[1] == node1_label)) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Upscaling graph:
 * 
 * Compute the new vertices and edges for a 2x higher resolution
 * Then, subdivide the edges in 2 smaller edges. Also add random offset to the intermediate vertex
 * to ensure the result doesnt contain lots of straight lines
 *
 * @param[in, out] graph  DLA graph
 */
void DLAGenerator::upscaleGraph(Graph& graph) {
    // Update position of graph nodes to match the new resolution (y and x coordinates of the nodes)

    for (size_t i = 1; i < graph.nodes_list_.size(); i++) {
        graph.nodes_list_[i]->y_ *= 2;
        graph.nodes_list_[i]->x_ *= 2;
    }

    std::vector<std::array<int, 2>> processed_edges = {};
    std::vector<std::array<int, 2>> edges_to_add = {};

    size_t low_res_graph_size = graph.nodes_list_.size();

    for (size_t i = 1; i < low_res_graph_size; i++) {
        size_t node_degree = graph.adjacency_list_[i].size();

        for (size_t j = 0; j < node_degree; j++) {
            std::shared_ptr<Node> node1 = graph.nodes_list_[i];
            std::shared_ptr<Node> node2 = graph.nodes_list_[graph.adjacency_list_[i][j]];

            if (edgeAlreadyProcessed(processed_edges, node1->label_, node2->label_)) {
                continue;
            }

            // Create a new node in the middle of the edge

            float middle_y = (node1->y_ + node2->y_) / 2;
            float middle_x = (node1->x_ + node2->x_) / 2;

            // Add random offset to intermediate points and round coordinates [-0.25, 0.25)

            float offset_y = real_dist1_zero_centered_(rng_) / 4;
            float offset_x = real_dist1_zero_centered_(rng_) / 4;
            middle_y += offset_y;
            middle_x += offset_x;

            // Add the middle node to the graph and the grid

            int middle_node_label = graph.nodes_list_.size();
            graph.nodes_list_.push_back(std::make_shared<Node>(middle_node_label, middle_y, middle_x, -1.0f));
            graph.adjacency_list_.push_back({});

            edges_to_add.push_back({ node1->label_, middle_node_label });
            edges_to_add.push_back({ middle_node_label, node2->label_ });

            // Store the processed edge to avoid processing it again (undirected graph)

            processed_edges.push_back({ node1->label_, node2->label_ });
        }
    }

    graph.removeEdges(processed_edges);
    graph.addEdges(edges_to_add);
}

/**
 * @brief Convolution of a grid with a square kernel, with odd length
 * We skip the outermost pixels of the grid (border) to avoid out of bounds access
 * -> works because we work with "island" shaped grids so we should not care about the borders
 *
 * @param[in, out] grid  grid to convolve
 * @param[in] kernel     square kernel to use for the convolution
 */
void convolution(Heightmap& grid, const std::vector<std::vector<float>>& kernel) {
    int kernel_size = kernel.size();
    int kernel_center = std::floor(kernel_size / 2);

    for (int y = kernel_center; y < grid.height_ - kernel_center; y++) {
        for (int x = kernel_center; x < grid.width_ - kernel_center; x++) {
            float new_value = 0.0f;

            for (int i = 0; i < kernel_size; i++) {
                for (int j = 0; j < kernel_size; j++) {
                    new_value += kernel[i][j] * grid.at(y + i - kernel_center, x + j - kernel_center);
                }
            }

            grid.set(y, x, new_value);
        }
    }
}

/**
 * @brief Upscaling blurry grid:
 *
 * Use linear interpolation on small resolution grid to get a 2x higher resolution grid
 * Blur slighlty the higher resolution grid by using a convolution with a gaussian kernel
 *
 * @param[in] low_res_blurry_grid  low resolution grid (blurry) to upscale
 *
 * @return higher resolution (2x) version of the (blurry) grid passed as argument
 */
Heightmap DLAGenerator::upscaleBlurryGrid(const Heightmap& low_res_blurry_grid) {
    Heightmap high_res_grid = Heightmap(low_res_blurry_grid.width_ * 2, low_res_blurry_grid.height_ * 2);

    // Use linear interpolation on small resolution grid to get a 2x higher resolution grid
    for (int y = 0; y < high_res_grid.height_; y++) {
        for (int x = 0; x < high_res_grid.width_; x++) {
            float low_res_y = static_cast<float>(y) / 2;
            float low_res_x = static_cast<float>(x) / 2;

            int y_ceil_pos = (std::ceil(low_res_y) < low_res_blurry_grid.height_) ? std::ceil(low_res_y) : low_res_blurry_grid.height_ - 1;
            int x_ceil_pos = (std::ceil(low_res_x) < low_res_blurry_grid.width_) ? std::ceil(low_res_x) : low_res_blurry_grid.width_ - 1;

            if (low_res_y == std::floor(low_res_y) && low_res_x == std::floor(low_res_x)) {
                high_res_grid.set(y, x, low_res_blurry_grid.at(low_res_y, low_res_x));
            } else if (low_res_y == std::floor(low_res_y)) {
                high_res_grid.set(y, x, (low_res_blurry_grid.at(low_res_y, std::floor(low_res_x)) + low_res_blurry_grid.at(low_res_y, x_ceil_pos)) / 2);
            } else if (low_res_x == std::floor(low_res_x)) {
                high_res_grid.set(y, x, (low_res_blurry_grid.at(std::floor(low_res_y), low_res_x) + low_res_blurry_grid.at(y_ceil_pos, low_res_x)) / 2);
            } else {
                float top_left = low_res_blurry_grid.at(std::floor(low_res_y), std::floor(low_res_x));
                float top_right = low_res_blurry_grid.at(std::floor(low_res_y), x_ceil_pos);
                float bottom_left = low_res_blurry_grid.at(y_ceil_pos, std::floor(low_res_x));
                float bottom_right = low_res_blurry_grid.at(y_ceil_pos, x_ceil_pos);

                high_res_grid.set(y, x, (top_left + top_right + bottom_left + bottom_right) / 4);
            }
        }
    }

    // Blur slightly the higher resolution grid by using a convolution with a gaussian kernel
    std::vector<std::vector<float>> gaussian_kernel_3x3 = {
        { 1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f },
        { 2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f },
        { 1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f }
    };

    std::vector<std::vector<float>> gaussian_kernel_5x5 = {
        { 1.0f / 273.0f, 4.0f / 273.0f, 7.0f / 273.0f, 4.0f / 273.0f, 1.0f / 273.0f },
        { 4.0f / 273.0f, 16.0f / 273.0f, 26.0f / 273.0f, 16.0f / 273.0f, 4.0f / 273.0f },
        { 7.0f / 273.0f, 26.0f / 273.0f, 41.0f / 273.0f, 26.0f / 273.0f, 7.0f / 273.0f },
        { 4.0f / 273.0f, 16.0f / 273.0f, 26.0f / 273.0f, 16.0f / 273.0f, 4.0f / 273.0f },
        { 1.0f / 273.0f, 4.0f / 273.0f, 7.0f / 273.0f, 4.0f / 273.0f, 1.0f / 273.0f }
    };

    // convolution(high_res_grid, gaussian_kernel_3x3);
    convolution(high_res_grid, gaussian_kernel_5x5);

    return high_res_grid;
}

/**
 * @brief Get the depth levels of the graph (BFS traversal)
 *
 * @param[in] graph  graph representation of the pixels of the grid
 *
 * @return vector of sets of node labels representing the depth levels of the graph,
 * ordered from root to deepest level (left to right in the vector)
 */
std::vector<std::set<int>> getGraphDepthLevels(const Graph& graph) {
    std::vector<std::set<int>> levels;

    std::vector<bool> visited_nodes(graph.nodes_list_.size(), false);
    std::queue<int> nodes_queue;

    visited_nodes[0] = true; // dummy node
    visited_nodes[1] = true; // first node

    nodes_queue.push(1); // first node
    nodes_queue.push(-1); // level separator

    std::set<int> current_level;

    while (!nodes_queue.empty()) {
        int current_node_label = nodes_queue.front();
        if (current_node_label == -1) {
            nodes_queue.pop();
            if (!nodes_queue.empty()) {
                nodes_queue.push(-1);
            }
            levels.push_back(current_level);
            current_level.clear();
            continue;
        }
        nodes_queue.pop();

        current_level.insert(current_node_label);

        for (int adjacent_label : graph.adjacency_list_[current_node_label]) {
            if (!visited_nodes[adjacent_label]) {
                visited_nodes[adjacent_label] = true;
                nodes_queue.push(adjacent_label);
            }
        }
    }

    return levels;
}

void tmpExportLevels(const std::vector<std::set<int>>& levels) {
    std::ofstream file("../images/DLA/DLA_levels.txt");
    if (!file.is_open()) {
        throw std::runtime_error("DLAGenerator: tmpExportLevels: Could not open file ../images/DLA/DLA_levels.txt");
    }

    for (size_t i = 0; i < levels.size(); i++) {
        file << "Level " << i << ": ";
        for (int label : levels[i]) {
            file << label << " ";
        }
        file << std::endl;
    }

    file.close();
}

/**
 * @brief Process graph height values to assign higher values to pixels the nearer to the center they are on the blurry grid:
 * 
 * Use height values (first "integers", then at the end converted to floats using the smooth falloff formula)
 * 
 * Assign outermost pixels value 1
 * Assign each pixel the maximum value of pixels that are downstream from it + 1
 * Use smooth falloff formula: 1 - 1 / (1 + h), to assign a height value (float) from the value (int) to the pixel
 *
 * @param[in, out] graph  graph representation of the pixels of the (crisp) grid
 */
void DLAGenerator::setGraphHeightValues(Graph& graph) {
    for (size_t i = 1; i < graph.nodes_list_.size(); i++) {
        graph.nodes_list_[i]->height_ = -1.0f;
    }

    std::vector<std::set<int>> levels = getGraphDepthLevels(graph);
    // tmpExportLevels(levels);

    if (levels.size() == 0) {
        throw std::runtime_error("DLAGenerator: setGraphHeightValues: No levels found in the graph");
    }

    for (int label : levels[levels.size() - 1]) {
        graph.nodes_list_[label]->height_ = 1.0f;
    }

    for (int i = levels.size() - 2; i >= 0; i--) {
        for (int label : levels[i]) {
            if (graph.adjacency_list_[label].size() == 1) {
                graph.nodes_list_[label]->height_ = 1.0f;
            } else {
                float max_height = -1.0f;

                for (int adjacent_label : graph.adjacency_list_[label]) {
                    if (levels[i + 1].contains(adjacent_label)) {
                        max_height = std::max(max_height, graph.nodes_list_[adjacent_label]->height_);
                    }
                }

                if (max_height == -1.0f) {
                    throw std::runtime_error("DLAGenerator: setGraphHeightValues: Could not find a downstream node for node " + std::to_string(label));
                }

                graph.nodes_list_[label]->height_ = max_height + 1.0f;
            }
        }
    }

    for (size_t i = 1; i < graph.nodes_list_.size(); i++) {
        if (graph.nodes_list_[i]->height_ == -1.0f) {
            throw std::runtime_error("DLAGenerator: setGraphHeightValues: Some nodes have not been assigned a height value");
        }
    }

    // graph.exportNodesHeight("../images/DLA/DLA_nodes_height.txt");

    // smooth fall-off formula: 1 - 1 / (1 + h)
    auto smooth_falloff = [](int h) -> float {
        return 1.0f - 1.0f / (1.0f + static_cast<float>(h));
    };

    // for every node we convert the "integer" height value to the real height value using the smooth falloff formula
    for (size_t i = 1; i < graph.nodes_list_.size(); i++) {
        graph.nodes_list_[i]->height_ = smooth_falloff(graph.nodes_list_[i]->height_);
    }

    // graph.exportNodesHeight("../images/DLA/DLA_nodes_height.txt");
}

/**
 * @brief Add height values to the blurry grid using the graph representation
 *
 * -> override the pixels of the blurry grid that are in the crisp grid with
 * their height value in the graph
 *
 * @param[in, out] blurry_grid  grid to add height values to
 * @param[in] graph             graph representation of the pixels of the (crisp) grid that hold the height values
 */
void addHeightToBlurryGrid(Heightmap& blurry_grid, const Graph& graph) {
    for (size_t i = 1; i < graph.nodes_list_.size(); i++) {
        float graph_height = graph.nodes_list_[i]->height_;
        float blurry_grid_height = blurry_grid.at(graph.nodes_list_[i]->y_, graph.nodes_list_[i]->x_);

        blurry_grid.set(graph.nodes_list_[i]->y_, graph.nodes_list_[i]->x_, blurry_grid_height + graph_height); // FIXME check if it is correct to add
    }
}

/**
 * @brief Convert the graph to a square heightmap using the height values of the nodes using linear interpolation.
 * TODO Later pls clean this function for my sanity.
 *
 * @param[in] width  width of the resulting heightmap
 * @param[in] graph  DLA graph
 *
 * @return square heightmap representing the height values of the nodes of the graph
 */
Heightmap DLAGenerator::graphToHeightmap(int width, const Graph& graph) {
    Heightmap heightmap = Heightmap(width, width);

    for (size_t i = 1; i < graph.nodes_list_.size(); i++) {
        float height = graph.nodes_list_[i]->height_;

        int y_int = static_cast<int>(graph.nodes_list_[i]->y_);
        float y_decimal = graph.nodes_list_[i]->y_ - y_int;
        int x_int = static_cast<int>(graph.nodes_list_[i]->x_);
        float x_decimal = graph.nodes_list_[i]->x_ - x_int;

        float y_weight = 0.5f - ((y_decimal < 0.5f) ? y_decimal : 1.0f - y_decimal);
        float x_weight = 0.5f - ((x_decimal < 0.5f) ? x_decimal : 1.0f - x_decimal);
        float yx_weight = y_weight * x_weight;

        if (y_decimal < 0.5f) { // up
            if (y_int - 1 >= 0) {
                heightmap.set(y_int - 1, x_int, heightmap.at(y_int - 1, x_int) + height * (y_weight - yx_weight)); // up
            }
            heightmap.set(y_int, x_int, heightmap.at(y_int, x_int) + height * (1.0f - y_weight - x_weight + yx_weight)); // center

            if (x_decimal < 0.5f) { // left
                if (y_int - 1 >= 0 && x_int - 1 >= 0) {
                    heightmap.set(y_int - 1, x_int - 1, heightmap.at(y_int - 1, x_int - 1) + height * yx_weight); // up left
                }
                if (x_int - 1 >= 0) {
                    heightmap.set(y_int, x_int - 1, heightmap.at(y_int, x_int - 1) + height * (x_weight - yx_weight)); // left
                }
            } else { // right
                if (y_int - 1 >= 0 && x_int + 1 < width) {
                    heightmap.set(y_int - 1, x_int + 1, heightmap.at(y_int - 1, x_int + 1) + height * yx_weight); // up right
                }
                if (x_int + 1 < width) {
                    heightmap.set(y_int, x_int + 1, heightmap.at(y_int, x_int + 1) + height * (x_weight - yx_weight)); // right
                }
            }
        } else { // down
            if (y_int + 1 < width) {
                heightmap.set(y_int + 1, x_int, heightmap.at(y_int + 1, x_int) + height * (y_weight - yx_weight)); // down
            }
            heightmap.set(y_int, x_int, heightmap.at(y_int, x_int) + height * (1.0f - y_weight - x_weight + yx_weight)); // center

            if (x_decimal < 0.5f) { // left
                if (y_int + 1 < width && x_int - 1 >= 0) {
                    heightmap.set(y_int + 1, x_int - 1, heightmap.at(y_int + 1, x_int - 1) + height * yx_weight); // down left
                }
                if (x_int - 1 >= 0) {
                    heightmap.set(y_int, x_int - 1, heightmap.at(y_int, x_int - 1) + height * (x_weight - yx_weight)); // left
                }
            } else { // right
                if (y_int + 1 < width && x_int + 1 < width) {
                    heightmap.set(y_int + 1, x_int + 1, heightmap.at(y_int + 1, x_int + 1) + height * yx_weight); // down right
                }
                if (x_int + 1 < width) {
                    heightmap.set(y_int, x_int + 1, heightmap.at(y_int, x_int + 1) + height * (x_weight - yx_weight)); // right
                }
            }
        }
    }

    return heightmap;
}

/**
 * @brief Main algorithm
 * 
 * Generate low resolution grid and populate it until a certain density threshold (populateGrid())
 * Repeat:
 *      Make 2 larger (higher resolution) grids from this one (2x resolution):
 *      - a blurry one
 *      - a crisp one
 *      
 *      Add more detail to the crisp one
 *      Use populated crisp image to add the new detail to the blurry version
 *
 * @param width  width of the final square heightmap (FOR NOW CHOOSE A POWER OF 2 (minimum will be 2^4 anyway))
 *
 * @return high resolution square heightmap representing a terrain (mountains)
 */
Heightmap DLAGenerator::generateUpscaledHeightmap(int width) {
    int power_of_two = 3;
    int base_width = 8; // 2^3

    Graph graph;

    // Add first real node to the graph 

    std::array<float, 2> pixel_coords = { graph_center_y_ * base_width, graph_center_x_ * base_width };
    int node_label = graph.nodes_list_.size(); // should be 1 (first actual node)
    graph.nodes_list_.push_back(std::make_shared<Node>(node_label, pixel_coords[0], pixel_coords[1], -1.0f));
    graph.adjacency_list_.push_back({});

    populateGraph(base_width, graph);
    setGraphHeightValues(graph);

    // Main loop

    Heightmap low_res_blurry_grid = graphToHeightmap(base_width, graph);
    Heightmap high_res_blurry_grid = low_res_blurry_grid;

    while (std::pow(2, power_of_two) < width) {
        // graph

        upscaleGraph(graph);
        Heightmap graph_heightmap = graphToHeightmap(std::pow(2, power_of_two + 1), graph); // useful for visualization
        populateGraph(std::pow(2, power_of_two + 1), graph);
        setGraphHeightValues(graph);
        std::cout << "Graph at level " << power_of_two + 1 << " done" << std::endl;

        // blurry grid

        high_res_blurry_grid = upscaleBlurryGrid(low_res_blurry_grid);
        addHeightToBlurryGrid(high_res_blurry_grid, graph);
        std::cout << "Blurry grid at level " << power_of_two + 1 << " done" << std::endl;

        // TODO DELETE only for debug
        Image2D graph_heightmap_image = Image2D(graph_heightmap);
        std::string filename_graph_heightmap = "../images/DLA/DLA_graph_heightmap_" + std::to_string(power_of_two + 1) + ".ppm";
        graph_heightmap_image.writePPM(filename_graph_heightmap.c_str(), false);

        Image2D blurry_grid_image = Image2D(high_res_blurry_grid);
        blurry_grid_image.minMaxNormalize();
        std::string filename_blurry = "../images/DLA/DLA_upscaled_blurry_" + std::to_string(power_of_two + 1) + ".ppm";
        blurry_grid_image.writePPM(filename_blurry.c_str(), false);
        // TODO DELETE END

        // loop management

        low_res_blurry_grid = high_res_blurry_grid;
        power_of_two++;
    }
    
    // TODO DELETE only for debug
    std::cout << graph.nodes_list_.size() << " nodes in the graph" << std::endl; // + 1 because of the dummy node
    std::cout << graph.adjacency_list_.size() << " adjacency lists" << std::endl; // + 1 because of the dummy node
    float density = static_cast<float>(graph.nodes_list_.size() - 1) / (std::pow(2, power_of_two) * std::pow(2, power_of_two));
    std::cout << "Density: " << density << std::endl;
    graph.exportToDot("../images/DLA/DLA_upscaled_graph.dot");
    // TODO DELETE END

    return high_res_blurry_grid;
}

/**
 * @brief Generate the base heightmap (for the normal map) and the upscaled heightmap (for textures).
 * The upscaled heightmap will be generated by the DLA algorithm. Base heightmap is a downsampled version of the upscaled heightmap.
 *
 * @param[out] base_heightmap      base heightmap (for the normal map), width must be a power of 2.
 * @param[out] upscaled_heightmap  upscaled heightmap (for textures), width must be a power of 2 (>= 2^4).
 */
void DLAGenerator::generateHeightmaps(Heightmap& base_heightmap, Heightmap& upscaled_heightmap) {
    upscaled_heightmap = generateUpscaledHeightmap(upscaled_heightmap.width_);
    base_heightmap = upscaled_heightmap.squareDownsample(base_heightmap.width_);
}

} // namespace DLA