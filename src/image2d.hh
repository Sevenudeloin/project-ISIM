#pragma once

#include <memory>
#include <vector>

#include "pixel.hh"

class Image2D
{
public:
    int width_;
    int height_;
    std::vector<std::shared_ptr<Pixel>> pixels_; // changer de collection ?

    Image2D();
    Image2D(int width, int height);

    void setPixel(const Pixel &pixel);
    void setPixel(int y, int x, double r, double g, double b);
    void setPixel(int y, int x, Color color);

    Color getPixel(int y, int x) const;

    void writePPM(const char *filename);
};