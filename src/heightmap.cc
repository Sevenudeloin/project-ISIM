#include "heightmap.hh"

Heightmap::Heightmap(int size)
    : size_(size)
{
    height_map_ = array2D(size_, std::vector<float>(size_, 0.0f));
}

float Heightmap::at(int y, int x) const
{
    return height_map_.at(y).at(x);
}

void Heightmap::set(int y, int x, float value)
{
    height_map_[y][x] = value;
}