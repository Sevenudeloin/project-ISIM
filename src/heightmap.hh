#pragma once

#include <string>
#include <vector>

#include "image2d.hh"

class Heightmap
{
public:
    using array2D = std::vector<std::vector<float>>;

    int width_;
    int height_;
    array2D height_map_;

    Heightmap(int width, int height);

    Heightmap(Image2D &img);

    Heightmap(const std::string &filename);

    float at(int y, int x) const;
    void set(int y, int x, float value);

    Image2D toImage2D();
    
    // works for square heightmaps
    Heightmap flattenSides(float flatness_amount);
};