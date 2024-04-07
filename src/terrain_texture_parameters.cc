#include "terrain_texture_parameters.hh"

#include <cmath>
#include <iostream>

#include "utils.hh"

TerrainTextureParameters::TerrainTextureParameters(
    std::list<std::tuple<double, Color>> normal_height_colors,
    Color above_color, double cliff_threshold, Color cliff_color,
    double beach_height, Color beach_color)
    : normal_height_colors_(normal_height_colors)
    , above_color_(above_color)
    , cliff_color_(cliff_color)
    , beach_height_(beach_height)
    , beach_color_(beach_color)
{
    cliff_threshold_ = std::sin(utils::degrees_to_radians(cliff_threshold));
}

TerrainTextureParameters::TerrainTextureParameters()
    : normal_height_colors_(std::list<std::tuple<double, Color>>())
    , above_color_(Color::fromRGB(255, 255, 255))
    , cliff_color_(Color::fromRGB(110, 92, 68))
    , beach_height_(0.05)
    , beach_color_(Color::fromRGB(237, 209, 154))
{
    cliff_threshold_ = std::sin(utils::degrees_to_radians(70));

    // Add grass texture layer
    normal_height_colors_.push_back(
        std::make_tuple(0.8, Color::fromRGB(117, 171, 106)));

    // Add rock texture layer
    normal_height_colors_.push_back(
        std::make_tuple(0.9, Color::fromRGB(110, 92, 68)));
}

Color TerrainTextureParameters::getTerrainColor(double height, Vector3 n,
                                                double sea_level) const
{
    if (n.y_ < cliff_threshold_)
    {
        return cliff_color_;
    }
    if (height < sea_level + beach_height_)
    {
        return beach_color_;
    }
    for (auto it = normal_height_colors_.begin();
         it != normal_height_colors_.end(); ++it)
    {
        if (height < std::get<0>(*it))
        {
            return std::get<1>(*it);
        }
    }
    return above_color_;
}