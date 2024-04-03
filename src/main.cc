#include <chrono>
#include <iostream>

#include "image2d.hh"
#include "rendering.hh"
#include "scene.hh"
#include "utils.hh"

int main()
{
    /*
    // Image

    auto aspect_ratio = 4.0 / 4.0;
    int image_width = 1025; // should be a (power of 2) + 1 for diamond-square

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    Image2D image(image_width, image_height);

    // simplex noise test

    for (int j = 0; j < image_height; j++)
    {
        for (int i = 0; i < image_width; i++)
        {
            float height = SimplexNoise::noise(i / 100.0, j / 100.0);

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
    */

    int image_height = 1080;
    int image_width = 1920;

    Image2D image(image_width, image_height);

    Scene test_scene = Scene::createTestScene(image_height, image_width);
    std::cout << "Scene created" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    Rendering::render(test_scene, image);
    std::cout << "Rendering done" << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Runtime : " << elapsed.count() << " seconds" << std::endl;

    image.writePPM("../images/output_scene_test.ppm");

    return 0;
}