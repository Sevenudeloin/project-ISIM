#include "wave_map_parameters.hh"

#include <cmath>

#include "utils.hh"

WaveMapParameters::WaveMapParameters(Color foam_color, double wave_freq,
                                     double wave_dist_decline,
                                     double wave_offset, double foam_threshold,
                                     double foam_normal_threshold)
    : foam_color_(foam_color)
    , wave_freq_(wave_freq)
    , wave_dist_decline_(wave_dist_decline)
    , wave_offset_(wave_offset)
    , foam_threshold_(foam_threshold)
{
    foam_normal_threshold =
        std::sin(utils::degrees_to_radians(foam_normal_threshold));
}

WaveMapParameters::WaveMapParameters()
    : foam_color_(Color(1.0, 1.0, 1.0, 1.0))
    , wave_freq_(1.0)
    , wave_dist_decline_(0.1)
    , wave_offset_(0.0)
    , foam_threshold_(0.1)
{
    foam_normal_threshold = std::sin(utils::degrees_to_radians(70));
}

Color WaveMapParameters::getWaveColor(double dist, Vector3 n) const
{
    if (n.y_ < foam_normal_threshold)
    {
        return foam_color_;
    }

    double wave = std::sin(wave_freq_ * dist + wave_offset_);
    double wave_decline = std::exp(-wave_dist_decline_ * dist);
    double wave_final = wave * wave_decline;
    double wave_height = std::min(0.0, wave_final - foam_threshold_);

    Color wave_color = foam_color_;
    wave_color.a_ = wave_height;
    return wave_color;
}