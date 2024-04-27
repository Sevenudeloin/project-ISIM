#include "dla_generator.hh"

#include <cmath>
#include <iostream>
#include <memory>

#include "heightmap.hh"

// Implementation of DLA Algorithm (intuition from https://youtu.be/gsJHzBTPG0Y?si=jipP7Z0xBVCW3Ip6):
// algorithm bounded to CPU (but we dont care)

namespace DLA {

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
 * @brief Get random coordinates for a pixel in a 2D grid
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

/**
 * @brief TODO
 *
 * @param[in, out] grid   grid to populate
 * @param[in, out] graph  graph representation of the pixels of the grid to populate
 */
void DLAGenerator::populateGrid(Heightmap& grid, Graph& graph) {
    std::array<int, 2> pixel_coords = getRandom2DPixelCoordinates(grid.width_, grid.height_); // { y, x }
    grid.height_map_[pixel_coords[0]][pixel_coords[1]] = 1; // FIXME: do i store 1 or the label of the node in the graph?
    graph.nodes_list_.push_back(std::make_shared<Node>(0, pixel_coords[0], pixel_coords[1], 1.0f));
    graph.adjacency_list_.push_back({});

    int pixels_count = 1;
    float density = static_cast<float>(pixels_count) / (grid.height_ * grid.width_);

    while (density < this->density_threshold_) {
        pixel_coords = getRandom2DPixelCoordinates(grid.width_, grid.height_); // { y, x }
        int y = pixel_coords[0];
        int x = pixel_coords[1];

        while (true) {
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

            // && works here because of short-circuit evaluation
            bool is_there_right_pixel = (x + 1 < grid.width_) && (grid.at(y, x + 1) == 1);
            bool is_there_left_pixel = (x - 1 >= 0) && (grid.at(y, x - 1) == 1);
            bool is_there_up_pixel = (y - 1 >= 0) && (grid.at(y - 1, x) == 1);
            bool is_there_down_pixel = (y + 1 < grid.height_) && (grid.at(y + 1, x) == 1);

            // check if the pixel is next to another pixel
            if (is_there_right_pixel || is_there_left_pixel || is_there_up_pixel || is_there_down_pixel) {
                // add it to the graph and continue
                //     - (if multiple pixels next to it, add edge to the one closest to the "center axis (x, y)"
                //       of the grid if the origin of this new basis is the center of the grid)

                grid.height_map_[y][x] = 1; // FIXME: do i store 1 or the label of the node in the graph?
                graph.nodes_list_.push_back(std::make_shared<Node>(static_cast<int>(graph.nodes_list_.size()), y, x, 1.0f));
                graph.adjacency_list_.push_back({});

                // if (is_there_right_pixel) {
                // } else if (is_there_left_pixel) {
                // } else if (is_there_up_pixel) {
                // } else if (is_there_down_pixel) {
                // }

                break;
            }
        }

        grid.height_map_[y][x] = 1;
        pixels_count++;
        density = static_cast<float>(pixels_count) / (grid.height_ * grid.width_);
    }
}

/**
 * @brief Main algorithm
 * 
 * Generate low resolution grid and populate it until a certain density threshold (populateGrid())
 * Repeat:
 *      Make 2 larger (higher resolution) grids from this one (i think 2x resolution):
 *      - a blurry one
 *      - a crisp one
 *      
 *      Add more detail to the crisp one (populateGrid())
 *      Use populated crisp image to add the new detail to the blurry version (FIXME this is unclear)
 *
 * @param width  width of the final square heightmap (FOR NOW CHOOSE A POWER OF 2 ABOVE 2^3)
 *
 * @return high resolution square heightmap representing a terrain (mountains)
 */
Heightmap DLAGenerator::generateUpscaledHeightmap(int width) {
    int power_of_two = 3;
    int base_width = 8; // 2^3

    Heightmap low_res_grid = Heightmap(base_width, base_width);
    populateGrid(low_res_grid);

    // Heightmap high_res_crisp_grid = // TODO;
    // Heightmap high_res_blurry_grid = // TODO;
    
    while (std::pow(2, power_of_two) <= width) {
        // TODO

        power_of_two++;
        // low_res_grid = high_res_crisp_grid;
        // high_res_crisp_grid = ;
    }
    
    return high_res_blurry_grid;
}


/**
 * Upscaling (crisp image):
 * 
 * Get graph representation of the grid (which pixels stuck to which one during population of the grid => TODO in populateGrid())
 * => maybe use adjacency matrix representation
 * Draw the graph edges on a higher resolution grid
 * => first subdivide the edges in 2 smaller edges (if we 2x the resolution each time)
 * => add random offset to intermediate points to ensure the result doesnt contain lots of straight lines)
 */

/**
 * Upscaling (blurry image):
 *
 * Use linear interpolation on small resolution grid to get a (2x probably) higher resolution grid
 * Blur slighlty the higher resolution grid by using a convolution with a gaussian? kernel
 */

/**
 * Make an island / moutain shape on the blurry image:
 * 
 * Use height values (first integers, then floats using the smooth falloff formula)
 * 
 * Assign outermost pixels value 1
 * Assign each pixel the maximum value of pixels that are downstream from it (using the graph representation) + 1
 * Use smooth falloff formula: 1 - 1 / (1 + h), to assign a height value (float) from the value (int) to the pixel
 */

}