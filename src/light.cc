#include "light.hh"

#include <iostream>

#include "utils.hh"

Light::Light(double intensity, const Color color)
    : intensity_(intensity)
    , color_(color)
{}

PointLight::PointLight(double intensity, const Color color, const Point3 center)
    : Light{ intensity, color }
    , center_(center)
{}

double PointLight::computeIntensity(const Ray &ray) const
{
    return intensity_ / (ray.direction_.length() * ray.direction_.length());
}

Vector3 PointLight::computeDir(Point3 p) const
{
    return Vector3::unit_vector(center_ - p);
}

AmbientLight::AmbientLight(double intensity, const Color color)
    : intensity_(intensity)
    , color_(color)
{}

Color AmbientLight::getAmbientLight() const
{
    return intensity_ * color_;
}