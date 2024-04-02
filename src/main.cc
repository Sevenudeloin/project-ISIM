#include <iostream>

#include "diamond-square.hh"
#include "image2d.hh"
#include "utils.hh"

int main()
{
    // Image

    auto aspect_ratio = 4.0 / 4.0;
    int image_width = 1025; // should be a (power of 2) + 1 for diamond-square

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    Image2D image(image_width, image_height);

    // diamond_square test

    int height_map_size = 1025; // should be a (power of 2) + 1
    algorithms::array2D height_map(height_map_size,
                                   std::vector<float>(height_map_size, 0.0f));

    // Set the initial values for the four corners
    height_map[0][0] = 0.5f; // Top left
    height_map[0][height_map_size - 1] = 0.5f; // Top right
    height_map[height_map_size - 1][0] = 0.5f; // Bottom left
    height_map[height_map_size - 1][height_map_size - 1] = 0.5f; // Bottom right

    algorithms::diamond_square(height_map, 0.7);

    for (int j = 0; j < image_height; j++)
    {
        for (int i = 0; i < image_width; i++)
        {
            float height = height_map[i][j];

            if (height < 0.0f || height > 1.0f)
            {
                std::clog << "Error: height out of bounds: " << height
                          << std::endl;
                return 1;
            }

            // std::clog << height << " ";
            Color pixel_color = Color(0.0, 0.0, 1.0);
            if (height > 0.3)
            {
                pixel_color = Color(0.0, 0.5, 0.0);
            }
            if (height > 0.6)
            {
                pixel_color = Color(0.5, 0.5, 0.5);
            }
            if (height > 0.8)
            {
                pixel_color = Color(1.0, 1.0, 1.0);
            }

            image.setPixel(i, j, pixel_color.r_, pixel_color.g_,
                           pixel_color.b_);
        }
        // std::clog << std::endl;
    }

    image.writePPM("output.ppm");

    return 0;
}