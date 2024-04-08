#pragma once

#include "color.hh"
#include "vector3.hh"

class WaveMapParameters
{
public:
    // For shore waves
    Color foam_color_; // Color of the waves
    double wave_freq_; // Frequency of the waves
    double wave_max_dist_;
    double wave_min_dist_;
    double wave_offset_; // Offset of the wave to the shore
    double foam_threshold_; // Threshold under which shore waves are not drawn

    // For deep ocean waves
    double foam_normal_min_threshold_; // Steepness threshold for foam start in
                                       // term of n_y, in [0, 1]
    double foam_normal_max_threshold_; // Steepness threshold for foam end in
                                       // term of n_y in [0, 1]

    WaveMapParameters(Color foam_color, double wave_freq, double wave_max_dist,
                      double wave_min_dist, double wave_offset,
                      double foam_threshold, double foam_normal_min_threshold,
                      double foam_normal_max_threshold);

    WaveMapParameters();

    Color getShoreWaveColor(double relative_height) const;
    Color getDeepOceanWaveColor(Vector3 n) const;
};