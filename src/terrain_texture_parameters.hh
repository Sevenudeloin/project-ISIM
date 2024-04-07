#pragma once

#include <list>
#include <tuple>

#include "color.hh"
#include "vector3.hh"

class TerrainTextureParameters
{
public:
    std::list<std::tuple<double, Color>> normal_height_colors_;

    Color above_color_; // color for height above the highest
                        // normal_height_colors_ (usually snow)

    double cliff_threshold_; // steepness threshold for cliffs in term of
                             // n_y [0, 1]
    Color cliff_color_; // color for cliffs

    double beach_height_; // height for the beach (relative positive height from
                          // the sea level)
    Color beach_color_; // color for the beach

    // cliff_threshold is in degrees [0, 90]
    TerrainTextureParameters(
        std::list<std::tuple<double, Color>> normal_height_colors,
        Color above_color, double cliff_threshold, Color cliff_color,
        double beach_height, Color beach_color);

    TerrainTextureParameters();

    Color getTerrainColor(double height, Vector3 n, double sea_level) const;
};