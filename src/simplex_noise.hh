#pragma once

#include <cstddef>

#include "generator.hh"
#include "heightmap.hh"

class SimplexNoiseGenerator : public Generator
{
public:
    SimplexNoiseGenerator();
    SimplexNoiseGenerator(size_t octaves, float frequency, float amplitude, float lacunarity, float persistence);

    // 1D Perlin simplex noise
    float noise(float x);
    // 2D Perlin simplex noise
    float noise(float x, float y);
    // 3D Perlin simplex noise
    float noise(float x, float y, float z);

    // Fractal/Fractional Brownian Motion (fBm) noise summation
    float fractal(float x); // 1D
    float fractal(float x, float y); // 2D
    float fractal(float x, float y, float z); // 3D

    // Heightmap generation
    // Heightmap generateHeightmap(float width, float height, int nbsamples_width, int nbsamples_height);
    Heightmap generateHeightmap(int width, int height, float scale, float offset_x, float offset_y, float offset_z);

private:
    // Parameters of Fractional Brownian Motion (fBm) : sum of N "octaves" of noise
    size_t octaves_;
    float frequency_;
    float amplitude_;
    float lacunarity_;
    float persistence_;
};

/**
 * The MIT License (MIT)
 * 
 * Copyright (c) 2012-2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */