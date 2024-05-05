#include "dla_generator.hh"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

#include "dla_graph.hh"
#include "heightmap.hh"

// Implementation of DLA Algorithm (intuition from https://youtu.be/gsJHzBTPG0Y?si=jipP7Z0xBVCW3Ip6):
// algorithm bounded to CPU (but we dont care)

namespace DLA {

std::uniform_int_distribution<std::mt19937::result_type> DLAGenerator::dist4_(1, 4);
// std::uniform_int_distribution<std::mt19937::result_type> DLAGenerator::dist5_(1, 5);

DLAGenerator::DLAGenerator()
    : rng_(rd_())
    , density_threshold_(0.5f)
{}

DLAGenerator::DLAGenerator(float density_threshold)
    : rng_(rd_())
    , density_threshold_(density_threshold)
{}

DLAGenerator::DLAGenerator(float density_threshold, int seed)
    : rng_(seed)
    , density_threshold_(density_threshold)
{}

/**
 * @brief Format for coordinates: { y, x }
 *
 * @param width   grid width
 * @param height  grid height
 *
 * @return random coordinates for a pixel in a 2D grid
 */
std::array<int, 2> DLAGenerator::getRandom2DPixelCoordinates(int width, int height) {
    std::uniform_int_distribution<std::mt19937::result_type> dist_height(0, height - 1);
    std::uniform_int_distribution<std::mt19937::result_type> dist_width(0, width - 1);

    return { static_cast<int>(dist_height(rng_)), static_cast<int>(dist_width(rng_)) };
}

float distanceToCenter(int width, int height, int y, int x) {
    int center_y = height / 2;
    int center_x = width / 2;
    return std::sqrt(std::pow(y - center_y, 2) + std::pow(x - center_x, 2));
}

/**
 * @brief Add pixels to the grid until a certain density threshold is reached. Pixels are spawned randomly and
 * move in a random cardinal direction until they are next to another pixel.
 * A node is then added to the graph with the pixel coordinates and a height value. The value of the pixel in
 * the grid is set to the node label (which is also the position in the nodes_list_ and adjacency_list_ of the
 * graph).
 * An edge is created between this node and the node corresponding to the pixel that it stuck to in the grid.
 * If there are multiple pixels next to it, the edge is created with the node of the pixel closest to the center
 * of the grid.
 *
 * @param[in, out] grid   grid to populate
 * @param[in, out] graph  graph representation of the pixels of the grid
 */
void DLAGenerator::populateGrid(Heightmap& grid, Graph& graph) {
    int pixels_count = grid.getAmountAboveThreshold(0);
    if (pixels_count == 0) {
        throw std::runtime_error("DLAGenerator: populateGrid: Need at least one pixel on the grid to populate it");
    }

    float density = static_cast<float>(pixels_count) / (grid.height_ * grid.width_);

    while (density < this->density_threshold_) {
        std::array<int, 2> pixel_coords = getRandom2DPixelCoordinates(grid.width_, grid.height_);
        int y = pixel_coords[0];
        int x = pixel_coords[1];

        // check if there is already a pixel at this position
        while (grid.at(y, x) != 0) {
            pixel_coords = getRandom2DPixelCoordinates(grid.width_, grid.height_);
            y = pixel_coords[0];
            x = pixel_coords[1];
        }

        while (true) {
            // check if the pixel is next to another pixel

            // (&& works here because of short-circuit evaluation)
            bool is_there_right_pixel = (x + 1 < grid.width_) && (grid.at(y, x + 1) > 0);
            bool is_there_left_pixel = (x - 1 >= 0) && (grid.at(y, x - 1) > 0);
            bool is_there_up_pixel = (y - 1 >= 0) && (grid.at(y - 1, x) > 0);
            bool is_there_down_pixel = (y + 1 < grid.height_) && (grid.at(y + 1, x) > 0);

            if (is_there_right_pixel || is_there_left_pixel || is_there_up_pixel || is_there_down_pixel) {
                // add it to the graph and continue the main loop
                // (if multiple pixels next to it, add edge to the one closest to the center of the grid)

                int node_label = graph.nodes_list_.size();
                grid.set(y, x, node_label);
                graph.nodes_list_.push_back(std::make_shared<Node>(node_label, y, x, 1.0f));
                graph.adjacency_list_.push_back({});

                float right_pixel_to_center = std::numeric_limits<float>::max();
                float left_pixel_to_center = std::numeric_limits<float>::max();
                float up_pixel_to_center = std::numeric_limits<float>::max();
                float down_pixel_to_center = std::numeric_limits<float>::max();

                if (is_there_right_pixel)
                    right_pixel_to_center = distanceToCenter(grid.width_, grid.height_, y, x + 1);
                if (is_there_left_pixel)
                    left_pixel_to_center = distanceToCenter(grid.width_, grid.height_, y, x - 1);
                if (is_there_up_pixel)
                    up_pixel_to_center = distanceToCenter(grid.width_, grid.height_, y - 1, x);
                if (is_there_down_pixel)
                    down_pixel_to_center = distanceToCenter(grid.width_, grid.height_, y + 1, x);

                float min_distance_to_center = std::min({ right_pixel_to_center, left_pixel_to_center, up_pixel_to_center, down_pixel_to_center });

                if (right_pixel_to_center == min_distance_to_center) {
                    graph.adjacency_list_[node_label].push_back(grid.at(y, x + 1));
                    graph.adjacency_list_[grid.at(y, x + 1)].push_back(node_label);
                } else if (left_pixel_to_center == min_distance_to_center) {
                    graph.adjacency_list_[node_label].push_back(grid.at(y, x - 1));
                    graph.adjacency_list_[grid.at(y, x - 1)].push_back(node_label);
                } else if (up_pixel_to_center == min_distance_to_center) {
                    graph.adjacency_list_[node_label].push_back(grid.at(y - 1, x));
                    graph.adjacency_list_[grid.at(y - 1, x)].push_back(node_label);
                } else if (down_pixel_to_center == min_distance_to_center) {
                    graph.adjacency_list_[node_label].push_back(grid.at(y + 1, x));
                    graph.adjacency_list_[grid.at(y + 1, x)].push_back(node_label);
                }

                break;
            }

            // move the pixel in a random cardinal direction

            int direction = dist4_(rng_); // random direction choice (1, 2, 3, 4)
            switch (direction) {
                case 1: // right
                    x = (x + 1 < grid.width_) ? (x + 1) : x;
                    break;
                case 2: // left
                    x = (x - 1 >= 0) ? (x - 1) : x;
                    break;
                case 3: // up
                    y = (y - 1 >= 0) ? (y - 1) : y;
                    break;
                case 4: // down
                    y = (y + 1 < grid.height_) ? (y + 1) : y;
                    break;
                default:
                    break;
            }
        }

        pixels_count++;
        density = static_cast<float>(pixels_count) / (grid.height_ * grid.width_);
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
 * @brief Upscaling crisp grid:
 * 
 * Use graph representation of the grid (which pixels stuck to which one during population of the grid)
 * to draw the graph edges on a higher resolution grid
 * => first subdivide the edges in 2 smaller edges (if we 2x the resolution each time)
 * => TODO add random offset to intermediate points to ensure the result doesnt contain lots of straight lines) => FIXME (UNCLEAR)
 *
 * @param[in] grid        low resolution grid (crisp)
 * @param[in, out] graph  graph representation of the pixels of the grid
 *
 * @return higher resolution (2x) version of the grid passed as argument (crisp)
 */
Heightmap DLAGenerator::upscaleCrispGrid(const Heightmap& low_res_crisp_grid, Graph& graph) {
    Heightmap high_res_grid = Heightmap(low_res_crisp_grid.width_ * 2, low_res_crisp_grid.height_ * 2);

    // update graph representation to match the new grid (y and x coordinates of the nodes)
    // and set grid values at the new coordinates to the label of the node
    for (size_t i = 1; i < graph.nodes_list_.size(); i++) {
        graph.nodes_list_[i]->y_ *= 2;
        graph.nodes_list_[i]->x_ *= 2;

        high_res_grid.set(graph.nodes_list_[i]->y_, graph.nodes_list_[i]->x_, graph.nodes_list_[i]->label_);
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

            int middle_y = std::round((node1->y_ + node2->y_) / 2);
            int middle_x = std::round((node1->x_ + node2->x_) / 2);

            // TODO: add random offset to intermediate points and round coordinates
            // int offset_y = dist4_(rng_) - 3; // -1, 0, 1
            // int offset_x = dist4_(rng_) - 3; // -1, 0, 1
            // middle_y += offset_y;
            // middle_x += offset_x;

            // Add the middle node to the graph and the grid

            int middle_node_label = graph.nodes_list_.size();
            graph.nodes_list_.push_back(std::make_shared<Node>(middle_node_label, middle_y, middle_x, 1.0f));
            graph.adjacency_list_.push_back({});

            edges_to_add.push_back({ node1->label_, middle_node_label });
            edges_to_add.push_back({ middle_node_label, node2->label_ });

            high_res_grid.set(middle_y, middle_x, middle_node_label);

            // Store the processed edge to avoid processing it again (undirected graph)

            processed_edges.push_back({ node1->label_, node2->label_ });
        }
    }

    graph.removeEdges(processed_edges);
    graph.addEdges(edges_to_add);

    return high_res_grid;
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

            if (low_res_y == std::floor(low_res_y) && low_res_x == std::floor(low_res_x)) {
                high_res_grid.set(y, x, low_res_blurry_grid.at(low_res_y, low_res_x));
            } else if (low_res_y == std::floor(low_res_y)) {
                high_res_grid.set(y, x, (low_res_blurry_grid.at(low_res_y, std::floor(low_res_x)) + low_res_blurry_grid.at(low_res_y, std::ceil(low_res_x))) / 2);
            } else if (low_res_x == std::floor(low_res_x)) {
                high_res_grid.set(y, x, (low_res_blurry_grid.at(std::floor(low_res_y), low_res_x) + low_res_blurry_grid.at(std::ceil(low_res_y), low_res_x)) / 2);
            } else {
                float top_left = low_res_blurry_grid.at(std::floor(low_res_y), std::floor(low_res_x));
                float top_right = low_res_blurry_grid.at(std::floor(low_res_y), std::ceil(low_res_x));
                float bottom_left = low_res_blurry_grid.at(std::ceil(low_res_y), std::floor(low_res_x));
                float bottom_right = low_res_blurry_grid.at(std::ceil(low_res_y), std::ceil(low_res_x));

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

    convolution(high_res_grid, gaussian_kernel_3x3);

    return high_res_grid;
}

/**
 * @brief Process graph height values to make an island / moutain shape on the blurry image:
 * 
 * Use height values (first integers, then at the end converted to floats using the smooth falloff formula)
 * 
 * Assign outermost pixels value 1
 * Assign each pixel the maximum value of pixels that are downstream from it + 1
 * Use smooth falloff formula: 1 - 1 / (1 + h), to assign a height value (float) from the value (int) to the pixel
 *
 * @param[in] grid        (crisp) grid used to compute the distance to the center
 * @param[in, out] graph  graph representation of the pixels of the (crisp) grid
 */
void setGraphHeightValues(const Heightmap& grid, Graph& graph) {
    // Assign outermost pixels height value 1
    for (size_t i = 1; i < graph.nodes_list_.size(); i++) {
        if (graph.adjacency_list_[i].size() == 1) {
            graph.nodes_list_[i]->height_ = 1.0f;
        }
    }

    // Assign each pixel the maximum value of pixels that are downstream from it + 1
    for (size_t i = 1; i < graph.nodes_list_.size(); i++) {
        float max_downstream_height = 1.0f;
        Node& current_node = *graph.nodes_list_[i];

        for (size_t j = 0; j < graph.adjacency_list_[i].size(); j++) {
            int adjacent_node_label = graph.adjacency_list_[i][j];
            Node& adjacent_node = *graph.nodes_list_[adjacent_node_label];
            
            // check if adjacent node is downstream (further away from the center of the grid)
            float current_distance_to_center = distanceToCenter(grid.width_, grid.height_, current_node.y_, current_node.x_);
            float adjacent_distance_to_center = distanceToCenter(grid.width_, grid.height_, adjacent_node.y_, adjacent_node.x_);
            if (adjacent_distance_to_center > current_distance_to_center) {
                float adjacent_node_height = graph.nodes_list_[adjacent_node_label]->height_;

                if (adjacent_node_height > max_downstream_height) {
                    max_downstream_height = adjacent_node_height;
                }
            }

        }

        graph.nodes_list_[i]->height_ = max_downstream_height + 1.0f;
    }

    // smooth fall-off formula: 1 - 1 / (1 + h)
    auto smooth_falloff = [](int h) -> float {
        return 1.0f - 1.0f / (1.0f + static_cast<float>(h));
    };

    // for every node we convert the "integer" height value to the real height value using the smooth falloff formula
    for (size_t i = 1; i < graph.nodes_list_.size(); i++) {
        graph.nodes_list_[i]->height_ = smooth_falloff(graph.nodes_list_[i]->height_);
    }
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
        blurry_grid.set(graph.nodes_list_[i]->y_, graph.nodes_list_[i]->x_, graph.nodes_list_[i]->height_);
    }
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
 * @param width  width of the final square heightmap (FOR NOW CHOOSE A POWER OF 2 (minimum will be 2^3 anyway))
 *
 * @return high resolution square heightmap representing a terrain (mountains)
 */
Heightmap DLAGenerator::generateUpscaledHeightmap(int width) {
    int power_of_two = 3;
    int base_width = 8; // 2^3

    Heightmap low_res_grid = Heightmap(base_width, base_width);
    Graph graph;

    // Add first pixel to the grid (also first real node of the graph) 

    std::array<int, 2> pixel_coords = getRandom2DPixelCoordinates(low_res_grid.width_, low_res_grid.height_);
    int node_label = graph.nodes_list_.size(); // should be 1 (first actual node)
    low_res_grid.set(pixel_coords[0], pixel_coords[1], node_label);
    graph.nodes_list_.push_back(std::make_shared<Node>(node_label, pixel_coords[0], pixel_coords[1], 1.0f));
    graph.adjacency_list_.push_back({});

    populateGrid(low_res_grid, graph);

    // Main loop

    Heightmap low_res_crisp_grid = low_res_grid;
    Heightmap low_res_blurry_grid = low_res_grid;

    Heightmap high_res_crisp_grid = low_res_crisp_grid;
    Heightmap high_res_blurry_grid = low_res_blurry_grid;
    while (std::pow(2, power_of_two) < width) {
        // crisp grid

        high_res_crisp_grid = upscaleCrispGrid(low_res_grid, graph);
        populateGrid(high_res_crisp_grid, graph);
        setGraphHeightValues(high_res_crisp_grid, graph);

        // blurry grid

        high_res_blurry_grid = upscaleBlurryGrid(low_res_blurry_grid);
        addHeightToBlurryGrid(high_res_blurry_grid, graph);

        // loop management

        low_res_crisp_grid = high_res_crisp_grid;
        low_res_blurry_grid = high_res_blurry_grid;
        power_of_two++;
    }
    
    // TODO DELETE only for debug
    int high_res_crisp_grid_amount = high_res_crisp_grid.getAmountAboveThreshold(0);
    std::cout << "Number of pixels in upscaled grid: " << high_res_crisp_grid_amount << std::endl;
    float high_res_crisp_grid_density = static_cast<float>(high_res_crisp_grid_amount) / (high_res_crisp_grid.height_ * high_res_crisp_grid.width_);
    std::cout << "Upscaled grid density: " << high_res_crisp_grid_density << std::endl;

    std::cout << graph.nodes_list_.size() << " nodes in the graph" << std::endl; // + 1 because of the dummy node
    std::cout << graph.adjacency_list_.size() << " adjacency lists" << std::endl; // + 1 because of the dummy node

    graph.exportToDot("../images/DLA/DLA_upscaled_graph.dot");
    // FIXME DELETE END

    return high_res_blurry_grid;
}

}