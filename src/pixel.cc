#include "pixel.hh"

Pixel::Pixel()
    : x_(0)
    , y_(0)
    , color_(Color())
{}

Pixel::Pixel(int x, int y, Color color)
    : x_(x)
    , y_(y)
    , color_(color)
{}

Pixel::Pixel(int x, int y, double r, double g, double b, double a)
    : x_(x)
    , y_(y)
    , color_(Color(r, g, b, a))
{}