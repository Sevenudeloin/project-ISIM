#include "heightmap.hh"

#include <algorithm>
#include <cmath>

#include "image2d.hh"
#include "ppm_parser.hh"

Heightmap::Heightmap(int width, int height)
    : width_(width)
    , height_(height)
{
    height_map_ = array2D(height, std::vector<float>(width, 0.0f));
}

Heightmap::Heightmap(Image2D &img)
    : width_(img.width_)
    , height_(img.height_)
{
    height_map_ = array2D(height_, std::vector<float>(width_, 0.0f));

    for (int y = 0; y < height_; y++)
    {
        for (int x = 0; x < width_; x++)
        {
            height_map_[y][x] = img.getPixel(y, x).r_ + img.getPixel(y, x).g_
                + img.getPixel(y, x).b_ / 3;
        }
    }
}

Heightmap::Heightmap(const std::string &filename)
{
    PPMParser parser(filename);

    Image2D img;
    parser.parse(img);

    *this = Heightmap(img);
}

float Heightmap::at(int y, int x) const
{
    return height_map_.at(y).at(x);
}

void Heightmap::set(int y, int x, float value)
{
    height_map_[y][x] = value;
}

Image2D Heightmap::toImage2D()
{
    Image2D image(width_, height_);

    for (int y = 0; y < height_; y++)
    {
        for (int x = 0; x < width_; x++)
        {
            float height = this->at(y, x);
            image.setPixel(y, x, height, height, height);
        }
    }

    return image;
}

/**
 * @brief Flattens the sides of the heightmap by multiplying it by a 2D gaussian distribution.
 * Works for square heightmaps (if width != height, the function will return a heightmap (width, width)).
 *
 * @param flatness_amount  How far the border is flatten [0, width/4[ ([0, base_sigma[)
 */
Heightmap Heightmap::flattenSides(float flatness_amount) // FIXME make flatness_amount a scaling parameter and divide base sigma by it when calling gaussian ?
{
    // FIXME modifies in place or returns a new heightmap? and should be member function or static ?
    Heightmap new_heightmap = Heightmap(width_, width_);
    Heightmap gaussian_heightmap = Heightmap(width_, width_);

    // Create a 2D gaussian distribution
    auto gaussian = [](float x, float y, float sigma) {
        return (1.0f / 2 * M_PIf * sigma * sigma) * std::exp(-(x * x + y * y) / (2 * sigma * sigma));
    };

    // Apply the gaussian distribution to the heightmap
    float base_sigma = static_cast<float>(width_) / 4; // 4 is kind of arbitrary, could find a better way to calculate base_sigma
    std::cout << "base_sigma: " << base_sigma << std::endl;

    for (int y = 0; y < width_; y++)
    {
        for (int x = 0; x < width_; x++)
        {
            float dx = x - width_ / 2;
            float dy = y - width_ / 2;
            float gaussian_value = gaussian(dx, dy, base_sigma - flatness_amount);

            // Normalize gaussian value to [0, 1]
            gaussian_value = gaussian_value / (gaussian(0, 0, base_sigma - flatness_amount));

            // (if we need to keep more height from the original heightmap starting from the middle)
            // Normalize to [0, a > 1] then clamp to [0, 1]
            // gaussian_value = std::clamp(gaussian_value * a, 0.0f, 1.0f);

            gaussian_heightmap.set(y, x, gaussian_value);

            float value = height_map_[y][x] * gaussian_value;
            new_heightmap.set(y, x, value);
        }
    }

    // for debug purposes
    // Image2D gaussian_image = gaussian_heightmap.toImage2D();
    // gaussian_image.writePPM("../images/heightmaps/gaussian.ppm");
    // Image2D new_image = new_heightmap.toImage2D();
    // new_image.writePPM("../images/heightmaps/flattened.ppm");

    return new_heightmap;
}