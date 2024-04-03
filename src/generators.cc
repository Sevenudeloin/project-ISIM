#include "generators.hh"

#include "heightmap.hh"
#include "simplex-noise.hh"

namespace generators {
    /**
     * Generate a 2D heightmap using Simplex noise and fBm with default parameters.
     * 
     * @param width    heightmap width
     * @param height   heightmap height 
     * @param octaves  number of fraction of noise to sum
     * @param x        float coordinate
     * @param y        float coordinate
     *
     * @return 2D heightmap
     */
    Heightmap generateSimplexNoise2D(int width, int height, size_t octaves, float x, float y)
    {
        SimplexNoise noise;

        Heightmap heightmap(width);
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                heightmap.set(j, i, noise.fractal(octaves, j / 100.0, i / 100.0)); // j / 100.0, i / 100.0 arbitrary sampling
            }
        }

        return heightmap;
    }

    /**
     * Generate a 2D heightmap using Simplex noise
     * 
     * @param width      heightmap width
     * @param height     heightmap height 
     * @param octaves    number of fraction of noise to sum
     * @param x          float coordinate
     * @param y          float coordinate
     * @param frequency  fBm frequency
     * @param amplitude  fBm amplitude
     * @param lacunarity fBm lacunarity
     * @param persistence fBm persistence
     *
     * @return 2D heightmap
     */
    Heightmap generateSimplexNoise2D(int width, int height, size_t octaves, float x, float y, float frequency, float amplitude, float lacunarity, float persistence)
    {
        SimplexNoise noise(frequency, amplitude, lacunarity, persistence);

        Heightmap heightmap(width);
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                heightmap.set(j, i, noise.fractal(octaves, j / 100.0, i / 100.0)); // j / 100.0, i / 100.0 arbitrary sampling
            }
        }

        return heightmap;
    }
}