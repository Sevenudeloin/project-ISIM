#include "diamond_square.hh"

#include <cmath>
#include <limits>

#include "utils.hh"

namespace algorithms {
    void diamond_step(array2D& height_map, int x, int y, int distance, float range)
    {
        float sum = 0.0f;
        int count = 0;
        int length = height_map.size();

        if (x + distance < length && y + distance < length)
        {
            // bottom right value
            sum += height_map[x + distance][y + distance];
            count++;
        }

        if (x + distance < length && y - distance >= 0)
        {
            // top right value
            sum += height_map[x + distance][y - distance];
            count++;
        }

        if (x - distance >= 0 && y - distance >= 0)
        {
            // top left value
            sum += height_map[x - distance][y - distance];
            count++;
        }

        if (x - distance >= 0 && y + distance < length)
        {
            // bottom left value
            sum += height_map[x - distance][y + distance];
            count++;
        }

        // set the value of the current point to the average of the surrounding points
        float avg = sum / count;
        avg += utils::random_float(-range, range);
        height_map[x][y] = avg;
    }

    // TODO implement wrapping and remove count mechanism
    // for wrapping we need opposite sides to store the same values
    void square_step(array2D& height_map, int x, int y, int distance, float range)
    {
        float sum = 0.0f;
        int count = 0;
        int length = height_map.size();

        if (x + distance < length)
        {
            // right value
            sum += height_map[x + distance][y];
            count++;
        }
        // TODO wrapping (get leftmost value)

        if (y - distance >= 0)
        {
            // top value
            sum += height_map[x][y - distance];
            count++;
        }
        // TODO wrapping (get bottom value)

        if (x - distance >= 0)
        {
            // left value
            sum += height_map[x - distance][y];
            count++;
        }
        // TODO wrapping (get rightmost value)

        if (y + distance < length)
        {
            // bottom value
            sum += height_map[x][y + distance];
            count++;
        }
        // TODO wrapping (get top value)

        // set the value of the current point to the average of the surrounding points
        float avg = sum / count;
        avg += utils::random_float(-range, range);
        height_map[x][y] = avg;
    }

    /* edge length of the array needs to be one greater than an integer power of two: n = 1+2**k, where k is a positive integer.
     * h is the roughness factor that we need to multiply the average by, h is between 0.0 (large) and 1.0 (small)
     * we need to set the corner values of the array to some initial values
     */
    void diamond_square(array2D& height_map, float h)
    {
        /*
        While the length of the side of the squares is greater than zero {
            Pass through the array and perform the diamond step for each square present.
            Pass through the array and perform the square step for each diamond present.
            Reduce the random number range.
        }
        */

        float range = 1.0f;
        int distance = std::log2(height_map.size() - 1); // distance is half the current length of the side of the squares

        while (distance > 0)
        {
            // diamond step
            for (size_t i = distance; i < height_map.size(); i += distance * 2)
            {
                for (size_t j = distance; j < height_map[i].size(); j += distance * 2)
                {
                    diamond_step(height_map, i, j, distance, range);
                }
            }

            // square step
            for (size_t i = distance; i < height_map.size(); i += distance * 2)
            {
                for (size_t j = 0; j < height_map[i].size(); j += distance * 2)
                {
                    square_step(height_map, i, j, distance, range);
                }
            }

            for (size_t i = 0; i < height_map.size(); i += distance * 2)
            {
                for (size_t j = distance; j < height_map[i].size(); j += distance * 2)
                {
                    square_step(height_map, i, j, distance, range);
                }
            }

            distance--;

            // multiply the random number range by the roughness factor (2^-h) and use the range in the subfunctions
            range = range * (1 / std::pow(2, h)); // 2^-h = 1/2^h
        }

        // normalize the height map to be between 0.0 and 1.0 for the image generation (FIXME remove later if needed)
        float min = std::numeric_limits<float>::infinity();

        for (size_t i = 0; i < height_map.size(); i++)
        {
            for (size_t j = 0; j < height_map[i].size(); j++)
            {
                if (height_map[i][j] < min)
                {
                    min = height_map[i][j];
                }
            }
        }

        float max = -std::numeric_limits<float>::infinity();

        for (size_t i = 0; i < height_map.size(); i++)
        {
            for (size_t j = 0; j < height_map[i].size(); j++)
            {
                if (height_map[i][j] > max)
                {
                    max = height_map[i][j];
                }
            }
        }

        for (size_t i = 0; i < height_map.size(); i++)
        {
            for (size_t j = 0; j < height_map[i].size(); j++)
            {
                height_map[i][j] = utils::normalize_float(height_map[i][j], min, max);
                height_map[i][j] = (height_map[i][j] + 1.0) / 2.0;
            }
        }
    }
}