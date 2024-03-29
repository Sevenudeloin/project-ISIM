#include "midpoint-displacement.hh"

#include <cstdlib>

#include "interval.hh"

namespace algorithms {
    void midpoint_displacement(std::vector<float>& height_map, float roughness)
    {
        std::srand(3);

        // Start with a single horizontal line segment
        for (size_t i = 0; i < height_map.size(); i++) {
            height_map[i] = 0.0;
        }

        // Repeat for a sufficiently large number of times
        for (size_t i = 0; i < 10; i++) {
            int step = height_map.size() / 2;
            while (step > 0) {
                for (size_t j = step; j < height_map.size(); j += step * 2) {
                    float avg = (height_map[j - step] + height_map[j + step]) / 2.0;
                    height_map[j] = avg + (std::rand() % 1000 / 1000.0 - 0.5) * roughness;
                    height_map[j] = Interval(-0.5, 0.5).clamp(height_map[j]);
                    height_map[j] = (height_map[j] + 1.0) / 2.0;
                }
                step /= 2;
            }
        }
    }
}