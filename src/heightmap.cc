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
 * @brief Return a new heightmap, which is the multiplication of the heightmap
 * by a 2D gaussian distribution. Works for square heightmaps (if width !=
 * height, the function will return a heightmap of dimension (width, width)).
 *
 * @param base_sigma  Standard deviation of the 2D gaussian
 */
Heightmap Heightmap::multiplyByGaussian(float base_sigma)
{
    Heightmap new_heightmap = Heightmap(width_, width_);
    Heightmap gaussian_heightmap = Heightmap(width_, width_);

    // Create a 2D gaussian distribution
    auto gaussian = [](float x, float y, float sigma) {
        return (1.0f / 2 * M_PIf * sigma * sigma)
            * std::exp(-(x * x + y * y) / (2 * sigma * sigma));
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

            // (if we need to keep more height from the original heightmap
            // starting from the middle) Normalize to [0, a > 1] then clamp to
            // [0, 1] float a = 2.0f; gaussian_value = std::clamp(gaussian_value
            // * a, 0.0f, 1.0f);

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
 * @brief Check if the sides of the heightmap are flat (under a certain
 * threshold).
 *
 * @param threshold  Threshold value to consider a side as flat
 * (default=std::numeric_limits<float>::epsilon())
 */
bool Heightmap::areSidesFlat(float threshold)
{
    for (int i = 0; i < width_; i++)
    {
        if (this->at(0, i) > threshold || this->at(width_ - 1, i) > threshold
            || this->at(i, 0) > threshold
            || this->at(i, width_ - 1) > threshold)
        {
            return false;
        }
    }

    return true;
}

/**
 * @brief Return a flattened heightmap, by multiplying the current instance by a
 * 2D gaussian distribution until the sides are under a certain threshold.
 *
 * @param threshold  Threshold value to consider a side as flat
 * (default=std::numeric_limits<float>::epsilon())
 */
Heightmap Heightmap::flattenSides(float threshold)
{
    Heightmap res_heightmap = *this;
    float base_sigma =
        static_cast<float>(width_) / 2; // TODO refine starting sigma value

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

void Heightmap::simulateErosion(int iterations, int drops, float initialWater,
                                float initialSpeed)
{
    for (int i = 0; i < drops; ++i)
    {
        Drop drop = initializeDrop(initialWater, initialSpeed);
        for (int j = 0; j < iterations; ++j)
        {
            if (!moveDrop(drop))
                break;
        }
    }
}

Drop Heightmap::initializeDrop(float initialWater, float initialSpeed)
{
    Drop drop;
    drop.pos = { static_cast<float>(std::rand() % width_),
                 static_cast<float>(std::rand() % height_) };
    drop.dir = { 0, 0 };
    drop.vel = initialSpeed;
    drop.water = initialWater;
    drop.sediment = 0;
    return drop;
}

Vector2 Heightmap::getGradient(const Vector2 &pos) const
{
    int x = static_cast<int>(pos.x);
    int y = static_cast<int>(pos.y);
    float u = pos.x - x;
    float v = pos.y - y;

    float height00 = at(y, x);
    float height10 = at(y, x + 1);
    float height01 = at(y + 1, x);
    float height11 = at(y + 1, x + 1);

    float gradX = (height10 - height00) * (1 - v) + (height11 - height01) * v;
    float gradY = (height01 - height00) * (1 - u) + (height11 - height10) * u;

    return { gradX, gradY };
}

bool Heightmap::moveDrop(Drop &drop)
{
    if (drop.water <= 0)
        return false;

    Vector2 gradient = getGradient(drop.pos);
    drop.dir = (drop.dir * 0.9f + gradient * 0.1f).normalized();
    Vector2 newPos = drop.pos + drop.dir * drop.vel;

    if (newPos.x < 0 || newPos.x >= width_ || newPos.y < 0
        || newPos.y >= height_)
        return false;

    float heightOld =
        at(static_cast<int>(drop.pos.y), static_cast<int>(drop.pos.x));
    float heightNew =
        at(static_cast<int>(newPos.y), static_cast<int>(newPos.x));

    if (heightNew > heightOld)
    {
        drop.vel *= 0.5f;
        drop.sediment =
            std::max(0.0f, drop.sediment - (heightNew - heightOld) * 0.1f);
    }
    else
    {
        drop.vel *= 1.1f;
        drop.sediment += (heightOld - heightNew) * 0.1f;
    }

    drop.pos = newPos;
    drop.water *= 0.99f;

    // Simulate deposition/erosion
    float deposit = drop.sediment * 0.1f;
    drop.sediment -= deposit;
    set(static_cast<int>(drop.pos.y), static_cast<int>(drop.pos.x),
        heightNew + deposit);

    return true;
}