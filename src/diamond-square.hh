#pragma once

#include <vector>

namespace algorithms {
    using array2D = std::vector<std::vector<float>>;

    // en soi les fonctions internes à l'algorithme diamond-square n'ont pas besoin d'être exportées ici
    void diamond_step(array2D& height_map, int x, int y, int distance, float range);
    void square_step(array2D& height_map, int x, int y, int distance, float range);

    void diamond_square(array2D& height_map, float h);
}