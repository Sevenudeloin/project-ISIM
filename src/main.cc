#include <cctype>
#include <chrono>
#include <iostream>
#include <sstream>
#include <unistd.h>

#include "heightmap.hh"
#include "image2d.hh"
#include "rendering.hh"
#include "scene.hh"
#include "simplex_noise.hh"
#include "utils.hh"

std::string capFirstLetter(std::string text)
{
    if (!text.empty()) {
        text[0] = toupper(text[0]);
    }

    return text;
}

int main(int argc, char* argv[])
{
    char opt;

    std::string output_filename = "../images/output.ppm" ;
    std::string dim_str;
    int image_width = 720;
    int image_height = 480;
    std::string scene_type = "test";
    bool show_help = false;

    while ((opt = getopt(argc, argv, "o:d:s:h")) != -1) {
        switch (opt) {
            case 'o':
                output_filename = optarg;
                break;
            case 'd':
                dim_str = optarg;
                break;
            case 's':
                scene_type = optarg;
                break;
            case 'h':
                show_help = true;
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " [-o <output_filename>] [-d <width>x<height>] [-s <scene_type>] [-h]" << std::endl;
                return 1;
        }
    }

    if (show_help) {
        std::cout << "Usage: " << argv[0] << " [-o <output_filename>] [-d <width>x<height>] [-s <scene_type>] [-h]" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "  -o <output_filename>  Specify the path to the output file (default is images/output.ppm)" << std::endl;
        std::cout << "  -d <width>x<height>   Specify the dimensions of the output image (default is 720x480)" << std::endl;
        std::cout << "  -s <scene_type>       Specify the scene (available: test, ocean, island), (default is test)" << std::endl;
        std::cout << "  -h                    Show this help menu" << std::endl;
        return 0;
    }

    if (!dim_str.empty()) {
        size_t pos = dim_str.find('x');
        if (pos != std::string::npos) {
            std::stringstream ss_width(dim_str.substr(0, pos));
            std::stringstream ss_height(dim_str.substr(pos + 1));
            ss_width >> image_width;
            ss_height >> image_height;
            if (image_width <= 0 || image_height <= 0) {
                std::cerr << "Error: Invalid size format or dimensions. Please use the format '<width>x<height>'." << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Error: Invalid size format. Please use the format '<width>x<height>'." << std::endl;
            return 1;
        }
    }

    Image2D image(image_width, image_height);

    Scene scene = Scene::createTestScene(image_height, image_width);

    if (scene_type == "ocean") {
        scene = Scene::createOceanScene(image_height, image_width);
    } else if (scene_type == "island") {
        scene = Scene::createIslandScene(image_height, image_width);
    }

    std::cout << capFirstLetter(scene_type) << " scene created" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    float scale = 35.f;
    float offset_x = 5.9f;
    float offset_y = 5.1f;
    float offset_z = 0.05f;
    SimplexNoiseGenerator simplexNoiseGenerator = SimplexNoiseGenerator(scale, 0.5f, 1.99f, 0.5f);
    Heightmap heightmap = simplexNoiseGenerator.generateHeightmap(720, 480, scale, offset_x, offset_y, offset_z);

    Image2D res = heightmap.to_image2D();

    // Rendering::render(scene, image);
    std::cout << "Rendering done" << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Runtime : " << elapsed.count() << " seconds" << std::endl;

    // image.writePPM(output_filename.c_str());
    res.writePPM(output_filename.c_str());

    return 0;
}