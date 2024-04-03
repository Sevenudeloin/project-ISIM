#include "image2d.hh"

#include <fstream>

#include "interval.hh"

Image2D::Image2D()
    : width_(0)
    , height_(0)
    , pixels_(std::vector<std::shared_ptr<Pixel>>())
{}

Image2D::Image2D(int width, int height)
    : width_(width)
    , height_(height)
    , pixels_(std::vector<std::shared_ptr<Pixel>>(width * height))
{
    for (int i = 0; i < width * height; i++)
    {
        pixels_[i] = std::make_shared<Pixel>(i % width, i / width, 0, 0,
                                             0); // TODO check if it works
    }
}

void Image2D::setPixel(const Pixel &pixel)
{
    pixels_[pixel.y_ * width_ + pixel.x_] = std::make_shared<Pixel>(pixel);
}

void Image2D::setPixel(int y, int x, double r, double g, double b)
{
    pixels_[y * width_ + x] = std::make_shared<Pixel>(x, y, r, g, b);
}

void Image2D::setPixel(int y, int x, Color color)
{
    setPixel(x, y, color.r_, color.g_, color.b_);
}

Color Image2D::getPixel(int y, int x) const
{
    return pixels_[y * width_ + x]->color_;
}

void Image2D::writePPM(const char *filename) // P3 format raw PPM
{
    std::ofstream file;
    file.open(filename);

    file << "P3\n";
    file << width_ << " " << height_ << "\n";
    file << "255\n";

    double r;
    double g;
    double b;

    for (int i = 0; i < width_ * height_; i++)
    {
        r = pixels_[i]->color_.r_;
        g = pixels_[i]->color_.g_;
        b = pixels_[i]->color_.b_;

        r = Color::linear_to_gamma(r);
        g = Color::linear_to_gamma(g);
        b = Color::linear_to_gamma(b);

        static const Interval intensity(0.000, 0.999);

        file << static_cast<int>(256 * intensity.clamp(r)) << " "
             << static_cast<int>(256 * intensity.clamp(g)) << " "
             << static_cast<int>(256 * intensity.clamp(b)) << "\n";
    }

    file.close();
}