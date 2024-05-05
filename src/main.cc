#include <cctype>
#include <chrono>
#include <iostream>
#include <sstream>
#include <unistd.h>

#include "dla_generator.hh"
#include "heightmap.hh"
#include "image2d.hh"
#include "rendering.hh"
#include "scene.hh"

std::string capFirstLetter(std::string text)
{
    if (!text.empty())
    {
        text[0] = toupper(text[0]);
    }

    return text;
}

void showHelpMenu(char* argv[]) {
    std::cout << "Usage: " << argv[0] << " [-o <output_filename>] [-d <width>x<height>] [-s <scene_type>] [-p] [-h]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -o <output_filename>  Specify the path to the output file (default is images/output.ppm)" << std::endl;
    std::cout << "  -d <width>x<height>   Specify the dimensions of the output image (default is 720x480)" << std::endl;
    std::cout << "  -s <scene_type>       Specify the scene (available: test, simplex), (default is test)" << std::endl;
    std::cout << "  -p                    Preview terrain heightmap only (available at images/heightmaps/heightmap_output.ppm)" << std::endl;
    std::cout << "  -h                    Show this help menu" << std::endl;
}

void tmpDLADebug() {
    std::cout << "Debug mode enabled" << std::endl;
    int width = 256;
    Heightmap grid = Heightmap(256, 256);
    DLA::DLAGenerator generator = DLA::DLAGenerator(0.3, 5); 

    // =====

    auto start = std::chrono::high_resolution_clock::now();
    Heightmap upscaled_grid = generator.generateUpscaledHeightmap(width);

    Heightmap normalized_grid = Heightmap(upscaled_grid);
    // find max height to normalize
    float max_height = 0.0f;
    for (int y = 0; y < width; y++) {
        for (int x = 0; x < width; x++) {
            if (normalized_grid.at(y, x) > max_height)
            {
                max_height = normalized_grid.at(y, x);
            }
        }
    }

    // normalize
    for (int y = 0; y < width; y++) {
        for (int x = 0; x < width; x++) {
            normalized_grid.set(y, x, normalized_grid.at(y, x) / max_height);
        }
    }

    Image2D normalized_grid_image = Image2D(normalized_grid);
    normalized_grid_image.writePPM("../images/DLA/DLA_test.ppm", false);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "full DLA runtime : " << elapsed.count() << " seconds" << std::endl;
}

int main(int argc, char *argv[])
{
    char opt;

    std::string output_filename = "../images/output.ppm" ;
    std::string dim_str;
    int image_width = 720;
    int image_height = 480;
    std::string scene_type = "test";
    bool only_preview = false;
    bool show_help = false;
    bool x_debug = false;

    while ((opt = getopt(argc, argv, "o:d:s:pxh")) != -1) {
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
            case 'p':
                only_preview = true;
                break;
            case 'x':
                x_debug = true;
                break;
            case 'h':
                show_help = true;
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " [-o <output_filename>] [-d <width>x<height>] [-s <scene_type>] [-p] [-h]" << std::endl;
                return 1;
        }
    }

    if (x_debug)
    {
        tmpDLADebug();
        return 0;
    }

    if (show_help) {
        showHelpMenu(argv);
        return 0;
    }

    if (!dim_str.empty()) {
        size_t pos = dim_str.find('x');
        if (pos != std::string::npos) {
            std::stringstream ss_width(dim_str.substr(0, pos));
            std::stringstream ss_height(dim_str.substr(pos + 1));
            ss_width >> image_width;
            ss_height >> image_height;
            if (image_width <= 0 || image_height <= 0)
            {
                std::cerr << "Error: Invalid size format or dimensions. Please "
                             "use the format '<width>x<height>'."
                          << std::endl;
                return 1;
            }
        }
        else
        {
            std::cerr << "Error: Invalid size format. Please use the format "
                         "'<width>x<height>'."
                      << std::endl;
            return 1;
        }
    }

    Image2D image(image_width, image_height);

    Scene scene = Scene::createTestScene(image_height, image_width);

    if (scene_type == "simplex")
    {
        scene = Scene::createSimplexScene(image_height, image_width);
    }

    // FIXME(not important): when invalid name is given, default to test scene but still prints 'invalid scene' message
    std::cout << capFirstLetter(scene_type) << " scene created" << std::endl;

    if (!only_preview)
    {
        auto start = std::chrono::high_resolution_clock::now();

        Rendering::render(scene, image);
        std::cout << "Rendering done" << std::endl;

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Runtime : " << elapsed.count() << " seconds" << std::endl;

        image.writePPM(output_filename.c_str(), true);
    }

    return 0;
}