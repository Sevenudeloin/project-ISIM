#include "image2d.hh"

#include <cmath>
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

Color Image2D::interpolate(float y, float x) const
{
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
        return Color(0, 0, 0);

    int x0 = static_cast<int>(x);
    int y0 = static_cast<int>(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    if (x1 >= width_)
        x1 = width_ - 1;
    if (y1 >= height_)
        y1 = height_ - 1;

    float dx = x - x0;
    float dy = y - y0;

    Color c00 = getPixel(y0, x0);
    Color c01 = getPixel(y0, x1);
    Color c10 = getPixel(y1, x0);
    Color c11 = getPixel(y1, x1);

    Color c0 = c00 * (1 - dx) + c01 * dx;
    Color c1 = c10 * (1 - dx) + c11 * dx;

    return c0 * (1 - dy) + c1 * dy;
}

Vector3 Image2D::getNormal(int y, int x) const
{
    Color col = interpolate(y, x);

    double n_x = -1 * (col.r_ - 0.5);
    double n_z = -1 * (col.g_ - 0.5);
    double n_y = std::sqrt(1 - n_x * n_x - n_z * n_z);

    return Vector3(n_x, n_y, n_z);
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