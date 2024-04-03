#include "light.hh"

Light::Light(double intensity, const Color color)
    : intensity_(intensity)
    , color_(color)
{}

PointLight::PointLight(double intensity, const Color color, const Point3 center)
    : Light{ intensity, color }
    , center_(center)
{}