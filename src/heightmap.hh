#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include <limits>

class Image2D;

class Heightmap
{
public:
    using array2D = std::vector<std::vector<float>>;

    int width_; // should be size_t :(
    int height_; // should be size_t :(
    array2D height_map_;

    Heightmap(int width, int height);

    Heightmap(Image2D &img);

    Heightmap(const std::string &filename);

    float at(int y, int x) const;
    void set(int y, int x, float value);

    void writeToFile(const std::string &filename);
    static Heightmap readFromFile(const std::string &filename);

    // The following methods only work for square heightmaps
    Heightmap multiplyByGaussian(float base_sigma);
    bool areSidesFlat(float threshold = std::numeric_limits<float>::epsilon());
    Heightmap flattenSides(float threshold = std::numeric_limits<float>::epsilon());

    int getAmountAboveThreshold(float threshold);

    Heightmap squareHalfDownsample();
    Heightmap squareDownsample(int width_threshold);
};