#pragma once

#include "color.hh"

class AbsorptionVolume
{
public:
    Color color_;

    AbsorptionVolume(Color color);

    virtual Color getAbsorptionColor(double t, Color hit_color) const;
    virtual double getTransmittance(double t) const = 0;
};

class LinearAbsorptionVolume : public AbsorptionVolume
{
public:
    double absorption_start_;
    double absorption_end_;
    double transmittance_min_;
    double fact_;
    double b_;

    LinearAbsorptionVolume(Color color, double absorption_start,
                           double absorption_end,
                           double transmittance_min_ = 0.0);

    double getTransmittance(double t) const override;
};

class ExponentialAbsorptionVolume : public AbsorptionVolume
{
public:
    double absorption_strength_;

    ExponentialAbsorptionVolume(Color color, double absorption_strength);

    double getTransmittance(double t) const override;
};