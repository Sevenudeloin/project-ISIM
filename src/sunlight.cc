#include <cmath>

#include "light.hh"
#include "material.hh"
#include "utils.hh"

SunLight::SunLight(double intensity, double lati, double longi)
    : Light(intensity, SunLight::base_lati_to_color(lati))
    , lati_(SunLight::lati_to_spherical(lati))
    , longi_(SunLight::longi_to_spherical(longi))
    , light_dir_(Vector3::spherical_to_cartesian(1.0, lati_, longi_))
{}

double SunLight::computeIntensity(Vector3) const
{
    return intensity_;
}

Vector3 SunLight::computeDir(Point3) const
{
    return light_dir_;
}

double SunLight::lati_to_spherical(double lati)
{
    return (utils::pi / 2) - utils::degrees_to_radians(lati);
}

double SunLight::longi_to_spherical(double longi)
{
    return utils::degrees_to_radians(longi);
}

// lati is in radians : 0 (up) to pi (down)
double SunLight::lati_to_temperature(double lati)
{
    if (lati < utils::pi / 2)
        return 1000.0 + 3000.0 * std::cos(lati);
    else
        return 6000;
}

Color SunLight::base_lati_to_color(double lati)
{
    lati = SunLight::lati_to_spherical(lati);
    return Color::temperature_to_color(SunLight::lati_to_temperature(lati));
}