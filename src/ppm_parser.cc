#include "ppm_parser.hh"

PPMParser::PPMParser(const std::string &filename)
    : filename_(filename)
{}

bool PPMParser::parse(Image2D &img)
{
    std::ifstream file(filename_, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Failed to open file " << filename_ << std::endl;
        return false;
    }

    std::string magic;
    file >> magic;
    if (magic != "P3")
    {
        std::cerr << "Error: Invalid PPM magic number. Expected P3."
                  << std::endl;
        return false;
    }

    file >> img.width_ >> img.height_;
    int max_val;
    file >> max_val;
    if (max_val != 255)
    {
        std::cerr << "Error: Unsupported max pixel value. Expected 255."
                  << std::endl;
        return false;
    }

    // Consume the newline character after the max value
    file.ignore(1);

    img.pixels_.resize(img.width_ * img.height_);

    for (int y = 0; y < img.height_; ++y)
    {
        for (int x = 0; x < img.width_; ++x)
        {
            int r, g, b;
            file >> r >> g >> b;
            img.setPixel(x, y,
                         Color(static_cast<double>(r) / 255.0,
                               static_cast<double>(g) / 255.0,
                               static_cast<double>(b) / 255.0));
        }
    }

    return true;
}