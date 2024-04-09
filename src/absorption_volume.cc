#include "absorption_volume.hh"

#include <cmath>

AbsorptionVolume::AbsorptionVolume(Color color)
    : color_(color)
{}

Color AbsorptionVolume::getAbsorptionColor(double t, Color hit_color) const
{
    double transmittance = getTransmittance(t);
    return (hit_color * transmittance) + ((1.0 - transmittance) * color_);
}

LinearAbsorptionVolume::LinearAbsorptionVolume(Color color,
                                               double absorption_start,
                                               double absorption_end,
                                               double transmittance_min)
    : AbsorptionVolume{ color }
    , absorption_start_(absorption_start)
    , absorption_end_(absorption_end)
    , transmittance_min_(transmittance_min)
    , fact_((transmittance_min - 1.0) / (absorption_end - absorption_start))
    , b_(1.0 - fact_ * absorption_start)
{}

double LinearAbsorptionVolume::getTransmittance(double t) const
{
    if (t < absorption_start_)
    {
        return 1.0;
    }
    else if (t > absorption_end_)
    {
        return transmittance_min_;
    }
    else
    {
        return fact_ * t + b_;
    }
}

ExponentialAbsorptionVolume::ExponentialAbsorptionVolume(
    Color color, double absorption_strength)
    : AbsorptionVolume{ color }
    , absorption_strength_(absorption_strength)
{}

double ExponentialAbsorptionVolume::getTransmittance(double t) const
{
    return std::exp(-absorption_strength_ * t);
}