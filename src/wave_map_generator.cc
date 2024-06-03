#include "wave_map_generator.hh"

#include <cmath>
#include <iostream>
#include <list>
#include <tuple>

#include "utils.hh"

Image2D WaveMapGenerator::generateShoreWaveMap(
    std::shared_ptr<Heightmap> terrain_height_map,
    const WaveMapParameters &params, double sea_level)
{
    Heightmap above_water_map =
        generateAboveWaterMap(terrain_height_map, sea_level);

    Heightmap dist_to_shore_map = generateDistToShoreMap(above_water_map);

    Image2D wave_map = distanceMapToWaveMap(dist_to_shore_map, params);

    wave_map.writePPM("../images/wave_map.ppm");

    return wave_map;
}

Image2D WaveMapGenerator::generateDeepOceanWaveMap(
    std::shared_ptr<Image2D> ocean_normal_map, const WaveMapParameters &params)
{
    Image2D wave_map(ocean_normal_map->width_, ocean_normal_map->height_);

    for (int i = 0; i < ocean_normal_map->height_; i++)
    {
        for (int j = 0; j < ocean_normal_map->width_; j++)
        {
            Vector3 n = ocean_normal_map->getNormal(i, j);
            Color wave_color = params.getDeepOceanWaveColor(n);
            wave_map.setPixel(i, j, wave_color);
        }
    }

    return wave_map;
}

Heightmap WaveMapGenerator::generateAboveWaterMap(
    std::shared_ptr<Heightmap> terrain_height_map, double sea_level)
{
    Heightmap above_water_map(terrain_height_map->width_,
                              terrain_height_map->height_);

    for (int i = 0; i < terrain_height_map->height_; i++)
    {
        for (int j = 0; j < terrain_height_map->width_; j++)
        {
            double height = terrain_height_map->at(i, j);
            above_water_map.set(i, j, height > sea_level ? 1.0 : 0.0);
        }
    }

    return above_water_map;
}

double dist(double x1, double y1, double x2, double y2)
{
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

Heightmap
WaveMapGenerator::generateDistToShoreMap(const Heightmap &above_water_map)
{
    int rows = above_water_map.height_;
    int cols = above_water_map.width_;
    Heightmap distances(rows, cols);

    std::list<std::pair<int, int>> above_pixels;
    std::list<std::pair<int, int>> other_pixels;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            double above = above_water_map.at(i, j);
            if (above > 0.0)
            {
                distances.set(i, j, 0.0);

                std::list<std::pair<int, int>> neighbors;
                if (i > 0)
                {
                    neighbors.push_back({ i - 1, j });
                }
                if (i < rows - 1)
                {
                    neighbors.push_back({ i + 1, j });
                }
                if (j > 0)
                {
                    neighbors.push_back({ i, j - 1 });
                }
                if (j < cols - 1)
                {
                    neighbors.push_back({ i, j + 1 });
                }

                bool all_neighbors_are_above = true;
                for (auto &n : neighbors)
                {
                    if (above_water_map.at(n.first, n.second) == 0.0)
                    {
                        all_neighbors_are_above = false;
                        break;
                    }
                }
                if (!all_neighbors_are_above)
                {
                    above_pixels.push_back({ i, j });
                }
            }
            else
            {
                other_pixels.push_back({ i, j });
            }
        }
    }

    for (auto &p : other_pixels)
    {
        double min_dist = std::numeric_limits<double>::max();
        for (auto &a : above_pixels)
        {
            double d = dist(p.first, p.second, a.first, a.second);
            if (d < min_dist)
            {
                min_dist = d;
            }
        }
        distances.set(p.first, p.second, min_dist);
    }

    return distances;
}

Image2D WaveMapGenerator::distanceMapToWaveMap(const Heightmap &distances,
                                               const WaveMapParameters &params)
{
    Image2D wave_map(distances.width_, distances.height_);

    for (int i = 0; i < distances.height_; i++)
    {
        for (int j = 0; j < distances.width_; j++)
        {
            double distance = distances.at(i, j);
            Color wave_color = params.getShoreWaveColor(distance);
            wave_color.r_ = wave_color.a_;
            wave_map.setPixel(i, j, wave_color);
        }
    }

    return wave_map;
}