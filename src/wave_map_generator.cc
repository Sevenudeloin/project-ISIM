#include "wave_map_generator.hh"

#include <cmath>
#include <queue>
#include <tuple>

#include "utils.hh"

Image2D
WaveMapGenerator::generateWaveMap(std::shared_ptr<Heightmap> terrain_height_map,
                                  std::shared_ptr<Image2D> ocean_normal_map,
                                  const WaveMapParameters &params,
                                  double sea_level)
{
    Heightmap above_water_map =
        generateAboveWaterMap(terrain_height_map, sea_level);

    Heightmap dist_to_shore_map = generateDistToShoreMap(above_water_map);

    Image2D wave_map =
        distanceMapToWaveMap(dist_to_shore_map, params, ocean_normal_map);

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

    std::queue<std::pair<int, int>> q;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            double above = above_water_map.at(i, j);
            if (above > 0.0)
            {
                distances.set(i, j, 0.0);
                q.push({ i, j });
            }
            else
            {
                distances.set(i, j, std::numeric_limits<float>::max());
            }
        }
    }

    while (!q.empty())
    {
        auto p = q.front();
        q.pop();

        int dx[] = { -1, 0, 1, 0 };
        int dy[] = { 0, -1, 0, 1 };

        for (int i = 0; i < 4; ++i)
        {
            int nx = p.first + dx[i];
            int ny = p.second + dy[i];

            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols
                && distances.at(ny, nx) == utils::infinity)
            {
                distances.set(ny, nx, dist(nx, ny, p.first, p.second));
                q.push({ nx, ny });
            }
        }
    }

    return distances;
}

Image2D WaveMapGenerator::distanceMapToWaveMap(
    const Heightmap &distances, const WaveMapParameters &params,
    std::shared_ptr<Image2D> ocean_normal_map)
{
    Image2D wave_map(distances.width_, distances.height_);

    for (int i = 0; i < distances.height_; i++)
    {
        for (int j = 0; j < distances.width_; j++)
        {
            double distance = distances.at(i, j);
            Vector3 n = ocean_normal_map->getNormal(i, j);
            Color wave_color = params.getWaveColor(distance, n);
            wave_map.setPixel(i, j, wave_color);
        }
    }

    return wave_map;
}