#pragma once

#include <memory>
#include <vector>

#include "pixel.hh"
#include "vector3.hh"

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

    Color interpolate(float y, float x, bool loop = false) const;
    Vector3 getNormal(double y, double x, bool raw = false) const;

    void minMaxNormalize();
    void sobelNormalize();

    void writePPM(const char *filename, bool gamma_correct = false) const;
};