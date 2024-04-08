#include <cctype>
#include <chrono>
#include <iostream>
#include <sstream>
#include <unistd.h>

#include "image2d.hh"
#include "rendering.hh"
#include "scene.hh"
#include "utils.hh"

std::string capFirstLetter(std::string text)
{
    if (!text.empty())
    {
        text[0] = toupper(text[0]);
    }

    return text;
}

int main(int argc, char *argv[])
{
    char opt;

    std::string output_filename = "../images/output.ppm";
    std::string size_str;
    int image_width = 720;
    int image_height = 480;
    std::string scene_type = "test";
    bool show_help = false;

    while ((opt = getopt(argc, argv, "o:s:t:h")) != -1)
    {
        switch (opt)
        {
        case 'o':
            output_filename = optarg;
            break;
        case 's':
            size_str = optarg;
            break;
        case 't':
            scene_type = optarg;
            break;
        case 'h':
            show_help = true;
            break;
        default:
            std::cerr << "Usage: " << argv[0]
                      << " [-o <output_filename>] [-s <width>x<height>] [-t "
                         "<scene_type>] [-h]"
                      << std::endl;
            return 1;
        }
    }

    if (show_help)
    {
        std::cout << "Usage: " << argv[0]
                  << " [-o <output_filename>] [-s <width>x<height>] [-t "
                     "<scene_type>] [-h]"
                  << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "  -o <output_filename>  Specify the path to the output "
                     "file (default is images/output.ppm)"
                  << std::endl;
        std::cout << "  -s <width>x<height>   Specify the size of the output "
                     "image (default is 720x480)"
                  << std::endl;
        std::cout << "  -t <scene_type>       Specify the scene type "
                     "(available: test, ocean, island), (default is test)"
                  << std::endl;
        std::cout << "  -h                    Show this help menu" << std::endl;
        return 0;
    }

    if (!size_str.empty())
    {
        size_t pos = size_str.find('x');
        if (pos != std::string::npos)
        {
            std::stringstream ss_width(size_str.substr(0, pos));
            std::stringstream ss_height(size_str.substr(pos + 1));
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

    /*
    if (scene_type == "ocean")
    {
        scene = Scene::createOceanScene(image_height, image_width);
    }
    else if (scene_type == "island")
    {
        scene = Scene::createIslandScene(image_height, image_width);
    }
    */

    std::cout << capFirstLetter(scene_type) << " scene created" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    Rendering::render(scene, image);
    std::cout << "Rendering done" << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Runtime : " << elapsed.count() << " seconds" << std::endl;

    image.writePPM(output_filename.c_str(), true);

    return 0;
}