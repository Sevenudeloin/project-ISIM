#pragma once

#include <cstdlib>
#include <cmath>
#include <limits>

namespace utils
{
    constexpr double infinity = std::numeric_limits<double>::infinity();
    constexpr double pi = 3.1415926535897932385;
    constexpr double kEpsilon = 1e-6;

    inline double degrees_to_radians(double degrees)
    {
        return degrees * pi / 180.0;
    }

    inline double random_double()
    {
        // Returns a random real in [0,1).
        return rand() / (RAND_MAX + 1.0);
    }

    inline double random_double(double min, double max)
    {
        // Returns a random real in [min,max).
        return min + (max - min) * random_double();
    }

    inline float random_float()
    {
        // Returns a random float in [0,1).
        return rand() / (RAND_MAX + 1.0);
    }

    inline float random_float(float min, float max)
    {
        // Returns a random float in [min,max).
        return min + (max - min) * random_float();
    }

    inline float normalize_float(float input, float min, float max)
    {
        float average = (min + max) / 2;
        float range = (max - min) / 2;
        float normalized = (input - average) / range;
        return normalized;
    }

    inline float euclidianDistance(float y1, float x1, float y2, float x2) {
        return std::sqrt(std::pow(y1 - y2, 2) + std::pow(x1 - x2, 2));
    }
} // namespace utils