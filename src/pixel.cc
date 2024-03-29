#include "pixel.hh"

Pixel::Pixel()
    : x_(0)
    , y_(0)
    , color_(Color(0.0, 0.0, 0.0))
{}

Pixel::Pixel(int x, int y, Color color)
    : x_(x)
    , y_(y)
    , color_(color)
{}

Pixel::Pixel(int x, int y, double r, double g, double b)
    : x_(x)
    , y_(y)
    , color_(Color(r, g, b))
{}