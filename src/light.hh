#pragma once

#include "color.hh"
#include "vector3.hh"

class Light
{
public:
    double intensity_;
    Color color_;

    Light(double intensity, const Color color);

    virtual double computeIntensity(Vector3 light_dir) const = 0;
};

class PointLight : public Light
{
public:
    Point3 center_;

    PointLight(double intensity, const Color color, const Point3 center);

    double computeIntensity(Vector3 light_dir) const override;
};