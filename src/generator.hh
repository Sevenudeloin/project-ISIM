#pragma once

#include "heightmap.hh"

class Generator
{
    virtual float noise(float x, float y) = 0;
    virtual Heightmap generateHeightmap(float width, float height, int nbsamples_width, int nbsamples_height) = 0;
};