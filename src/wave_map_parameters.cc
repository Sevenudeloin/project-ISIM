#include "wave_map_parameters.hh"

#include <cmath>

#include "utils.hh"

WaveMapParameters::WaveMapParameters(Color foam_color, double wave_freq,
                                     double wave_max_dist, double wave_min_dist,
                                     double wave_offset, double foam_threshold,
                                     double foam_normal_min_threshold,
                                     double foam_normal_max_threshold)
    : foam_color_(foam_color)
    , wave_freq_(wave_freq)
    , wave_max_dist_(wave_max_dist)
    , wave_min_dist_(wave_min_dist)
    , wave_offset_(wave_offset)
    , foam_threshold_(foam_threshold)
{
    foam_normal_min_threshold_ =
        std::sin(utils::degrees_to_radians(foam_normal_min_threshold));
    foam_normal_max_threshold_ =
        std::sin(utils::degrees_to_radians(foam_normal_max_threshold));
}

WaveMapParameters::WaveMapParameters()
    : foam_color_(Color(1.0, 1.0, 1.0, 1.0))
    , wave_freq_(1.0)
    , wave_max_dist_(15.0)
    , wave_min_dist_(55.0)
    , wave_offset_(10.0)
    , foam_threshold_(0.2)
{
    foam_normal_min_threshold_ = std::sin(utils::degrees_to_radians(75));
    foam_normal_max_threshold_ = std::sin(utils::degrees_to_radians(70));
}

Color WaveMapParameters::getShoreWaveColor(double dist) const
{
    double wave = std::sin(wave_freq_ * dist + wave_offset_);
    double wave_decline = 0;
    if (dist <= wave_max_dist_)
    {
        wave_decline = 1.0;
    }
    else if (dist <= wave_min_dist_)
    {
        double t = (dist - wave_max_dist_) / (wave_min_dist_ - wave_max_dist_);
        wave_decline = 1.0 - t;
    }
    double wave_final = wave * wave_decline;

    Color wave_color = foam_color_;
    wave_color.a_ = wave_final;
    return wave_color;
}

Color WaveMapParameters::getDeepOceanWaveColor(Vector3 n) const
{
    Color col = foam_color_;

    col.a_ = 1.0;
    if (n.y_ >= foam_normal_min_threshold_)
    {
        col.a_ = 0.0;
    }
    else if (n.y_ >= foam_normal_max_threshold_)
    {
        col.a_ = 1.0
            - (n.y_ - foam_normal_max_threshold_)
                / (foam_normal_min_threshold_ - foam_normal_max_threshold_);
    }

    return col;
}