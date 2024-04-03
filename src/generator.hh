#pragma once

#include "heightmap.hh"

class Generator
{
    virtual float noise(float x, float y) = 0;
    virtual Heightmap generateHeightmap(int width, int height) = 0;
};