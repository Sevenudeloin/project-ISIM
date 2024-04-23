#include "dla_generator.hh"

// TODO graph structure ?

// DLA is bounded to CPU (but we dont care anyway)

void DLAGenerator::populateGrid(Heightmap& grid, float density_threshold) {
    int pixels_count = 0;
    float density = static_cast<float>(pixels_count) / (grid.height_ * grid.width_);
}

/**
 * DLA Algorithm:
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
 * =====
 * 
 * Experiment with hyperparameters values to get different results
 */

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