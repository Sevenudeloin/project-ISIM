#pragma once

#include <cmath>
#include <vector>

#include "color.hh"
#include "ppm_parser.hh"
#include "utils.hh"
#include "vector3.hh"

class SkyBox
{
public:
    SkyBox()
    {}

    virtual ~SkyBox() = default;

    virtual Color getSkyboxAt(Vector3 dir) const = 0;
};

class SkyBoxGradient : public SkyBox
{
public:
    Color up_color_;
    Color horizon_color_;
    Color down_color_;

    SkyBoxGradient(Color up_color, Color horizon_color, Color down_color);

    Color getSkyboxAt(Vector3 dir) const override;
};

class SkyBoxImage : public SkyBox
{
public:
    std::vector<Color> pixels_;
    int width_;
    int height_;

    SkyBoxImage(const std::string &filename);

    Color getSkyboxAt(Vector3 dir) const override;
};