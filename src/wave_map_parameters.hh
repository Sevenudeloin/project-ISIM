#pragma once

#include "color.hh"
#include "vector3.hh"

class WaveMapParameters
{
public:
    // For shore waves
    Color foam_color_; // Color of the waves
    double wave_freq_; // Frequency of the waves
    double
        wave_dist_decline_; // Decline of the wave amplitude with the distance
    double wave_offset_; // Offset of the wave to the shore
    double foam_threshold_; // Threshold under which shore waves are not drawn

    // For deep ocean waves
    double foam_normal_threshold; // Steepness threshold for foam in term of n_y
                                  // [0, 1]

    WaveMapParameters(Color foam_color, double wave_freq,
                      double wave_dist_decline, double wave_offset,
                      double foam_threshold, double foam_normal_threshold);

    WaveMapParameters();

    Color getWaveColor(double relative_height, Vector3 n) const;
};