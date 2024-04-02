#pragma once

#include <vector>

class Heightmap
{
public:
    using array2D = std::vector<std::vector<float>>;

    int size_;
    array2D height_map_;

    Heightmap(int size);

    float at(int y, int x) const;
};