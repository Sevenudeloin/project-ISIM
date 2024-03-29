#pragma once

#include "color.hh"

struct Pixel
{
    int x_; // from the top left corner to the right
    int y_; // from the top left corner to the bottom
    Color color_;

    Pixel();
    Pixel(int x, int y, Color color);
    Pixel(int x, int y, double r, double g, double b);
};