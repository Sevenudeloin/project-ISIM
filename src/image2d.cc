#include "image2d.hh"

#include <cmath>
#include <fstream>
#include <iostream>

#include "interval.hh"
#include "utils.hh"

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
    setPixel(y, x, color.r_, color.g_, color.b_);
}

Color Image2D::getPixel(int y, int x) const
{
    return pixels_[y * width_ + x]->color_;
}

Color Image2D::interpolate(float y, float x, bool loop) const
{
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
        return Color(0, 0, 0);

    int x0 = static_cast<int>(x);
    int y0 = static_cast<int>(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    if (x1 >= width_)
    {
        if (loop)
            x1 = 0;
        else
            x1 = width_ - 1;
    }
    if (y1 >= height_)
    {
        if (loop)
            y1 = 0;
        else
            y1 = height_ - 1;
    }

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

Vector3 Image2D::getNormal(double y, double x, bool raw) const
{
    Color col = interpolate(y, x);

    if (raw)
        return Vector3::unit_vector(Vector3(col.r_, col.b_, col.g_));

    double n_x = -1 * (col.r_ - 0.5);
    double n_z = -1 * (col.g_ - 0.5);
    double n_y = std::sqrt(1 - n_x * n_x - n_z * n_z);

    return Vector3(n_x, n_y, n_z);
}

void Image2D::minMaxNormalize()
{
    double min = utils::infinity;
    double max = -utils::infinity;

    for (int i = 0; i < width_ * height_; i++)
    {
        double r = pixels_[i]->color_.r_;
        double g = pixels_[i]->color_.g_;
        double b = pixels_[i]->color_.b_;

        if (r < min)
            min = r;
        if (g < min)
            min = g;
        if (b < min)
            min = b;

        if (r > max)
            max = r;
        if (g > max)
            max = g;
        if (b > max)
            max = b;
    }

    for (int i = 0; i < width_ * height_; i++)
    {
        double r = pixels_[i]->color_.r_;
        double g = pixels_[i]->color_.g_;
        double b = pixels_[i]->color_.b_;

        r = (r - min) / (max - min);
        g = (g - min) / (max - min);
        b = (b - min) / (max - min);

        pixels_[i]->color_ = Color(r, g, b);
    }
}

void Image2D::sobelNormalize()
{
    double min = utils::infinity;
    double max = -utils::infinity;

    for (int i = 0; i < width_ * height_; i++)
    {
        double val = pixels_[i]->color_.r_;
        if (val < min)
            min = val;
        if (val > max)
            max = val;
    }

    double divisor = std::fmax(std::fabs(min), std::fabs(max));

    for (int i = 0; i < width_ * height_; i++)
    {
        double r = ((pixels_[i]->color_.r_ / divisor) + 1.0) / 2.0;
        double g = ((pixels_[i]->color_.r_ / divisor) + 1.0) / 2.0;
        double b = ((pixels_[i]->color_.r_ / divisor) + 1.0) / 2.0;

        pixels_[i]->color_ = Color(r, g, b);
    }
}

void Image2D::writePPM(const char *filename,
                       bool gamma_correct) const // P3 format raw PPM
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

        if (gamma_correct)
        {
            r = Color::linear_to_gamma(r);
            g = Color::linear_to_gamma(g);
            b = Color::linear_to_gamma(b);
        }

        static const Interval intensity(0.0, 1.0);

        file << static_cast<int>(255 * intensity.clamp(r)) << " "
             << static_cast<int>(255 * intensity.clamp(g)) << " "
             << static_cast<int>(255 * intensity.clamp(b)) << "\n";
    }

    file.close();
}