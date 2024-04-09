#include "normal_map_generator.hh"

#include <cmath>
#include <iostream>

#include "utils.hh"

Image2D
NormalMapGenerator::generateNormalMap(std::shared_ptr<Heightmap> height_map,
                                      double strength, double xy_scale)
{
    Image2D normal_map(height_map->width_, height_map->height_);
    Image2D sobelX(height_map->width_, height_map->height_);
    Image2D sobelY(height_map->width_, height_map->height_);

    // Matrices de convolution Sobel
    std::vector<std::vector<int>> kSobelX = { { -1, 0, 1 },
                                              { -2, 0, 2 },
                                              { -1, 0, 1 } };
    std::vector<std::vector<int>> kSobelY = { { -1, -2, -1 },
                                              { 0, 0, 0 },
                                              { 1, 2, 1 } };

    // Appliquer les filtres de Sobel en utilisant la convolution
    for (int i = 1; i < height_map->height_ - 1; ++i)
    {
        for (int j = 1; j < height_map->width_ - 1; ++j)
        {
            double gx = 0.0, gz = 0.0;
            for (int m = -1; m <= 1; ++m)
            {
                for (int n = -1; n <= 1; ++n)
                {
                    double height = height_map->at(i + m, j + n);
                    gx += height * kSobelX[m + 1][n + 1];
                    gz += height * kSobelY[m + 1][n + 1];
                }
            }
            gx = -1 * (gx * strength / (2 * xy_scale));
            gz = -1 * (gz * strength / (2 * xy_scale));
            sobelX.setPixel(i, j, gx, gx, gx);
            sobelY.setPixel(i, j, gz, gz, gz);
        }
    }

    // Calculer la normale
    for (int i = 0; i < height_map->height_; ++i)
    {
        for (int j = 0; j < height_map->width_; ++j)
        {
            double x = sobelX.getPixel(i, j).r_;
            double z = sobelY.getPixel(i, j).r_;
            Color c = Color(x, z, 1);
            normal_map.setPixel(i, j, c);
        }
    }

    return normal_map;
}