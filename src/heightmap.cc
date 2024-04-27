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

/**
 * @brief Return a new heightmap, which is the multiplication of the heightmap by a 2D gaussian distribution.
 * Works for square heightmaps (if width != height, the function will return a heightmap of dimension (width, width)).
 *
 * @param base_sigma  Standard deviation of the 2D gaussian
 */
Heightmap Heightmap::multiplyByGaussian(float base_sigma)
{
    Heightmap new_heightmap = Heightmap(width_, width_);
    Heightmap gaussian_heightmap = Heightmap(width_, width_);

    // Create a 2D gaussian distribution
    auto gaussian = [](float x, float y, float sigma) {
        return (1.0f / 2 * M_PIf * sigma * sigma) * std::exp(-(x * x + y * y) / (2 * sigma * sigma));
    };

    for (int y = 0; y < width_; y++)
    {
        for (int x = 0; x < width_; x++)
        {
            float dx = x - width_ / 2;
            float dy = y - width_ / 2;
            float gaussian_value = gaussian(dx, dy, base_sigma);

            // Normalize gaussian value to [0, 1]
            gaussian_value = gaussian_value / (gaussian(0, 0, base_sigma));

            // (if we need to keep more height from the original heightmap starting from the middle)
            // Normalize to [0, a > 1] then clamp to [0, 1]
            // float a = 2.0f;
            // gaussian_value = std::clamp(gaussian_value * a, 0.0f, 1.0f);

            gaussian_heightmap.set(y, x, gaussian_value);

            float value = height_map_[y][x] * gaussian_value;
            new_heightmap.set(y, x, value);
        }
    }

    // for debug purposes

    // Image2D gaussian_image = Image2D(gaussian_heightmap);
    // gaussian_image.writePPM("gaussian.ppm", false);
    // Image2D new_image = Image2D(new_heightmap);
    // new_image.writePPM("flattened.ppm", false);

    return new_heightmap;
}

/**
 * @brief Check if the sides of the heightmap are flat (under a certain threshold).
 *
 * @param threshold  Threshold value to consider a side as flat (default=std::numeric_limits<float>::epsilon())
 */
bool Heightmap::areSidesFlat(float threshold)
{
    for (int i = 0; i < width_; i++)
    {
        if (this->at(0, i) > threshold || this->at(width_ - 1, i) > threshold
            || this->at(i, 0) > threshold || this->at(i, width_ - 1) > threshold)
        {
            return false;
        }
    }

    return true;
}

/**
 * @brief Return a flattened heightmap, by multiplying the current instance by a 2D gaussian distribution until the sides are under a certain threshold.
 *
 * @param threshold  Threshold value to consider a side as flat (default=std::numeric_limits<float>::epsilon())
 */
Heightmap Heightmap::flattenSides(float threshold)
{
    Heightmap res_heightmap = *this;
    float base_sigma = static_cast<float>(width_) / 2; // TODO refine starting sigma value

    int debug_count = 0;

    while (!res_heightmap.areSidesFlat(threshold))
    {
        res_heightmap = this->multiplyByGaussian(base_sigma);

        std::cout << "base_sigma: " << base_sigma << std::endl;

        base_sigma /= 2; // TODO refine iteration process

        debug_count++;
    }

    std::cout << "Iterations: " << debug_count << std::endl;

    return res_heightmap;
} 

int Heightmap::getAmountAboveThreshold(float threshold)
{
    int count = 0;

    for (int y = 0; y < height_; y++)
    {
        for (int x = 0; x < width_; x++)
        {
            if (height_map_[y][x] > threshold)
            {
                count++;
            }
        }
    }

    return count;
}