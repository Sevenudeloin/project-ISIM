#include <iostream>

#include "image2d.hh"
#include "midpoint-displacement.hh"

int main()
{
    // Image

    auto aspect_ratio = 4.0 / 4.0;
    int image_width = 200;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    Image2D image(image_width, image_height);

    // tmp height map test

    std::vector<float> height_map(image_width * image_height, 0.0);
    algorithms::midpoint_displacement(height_map, 1.0);

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            float height = height_map[j * image_width + i];

            Color pixel_color = Color(height, height, height);

            image.setPixel(i, j, pixel_color.r_, pixel_color.g_, pixel_color.b_);
        }
    }

    image.writePPM("output.ppm");

    return 0;
}