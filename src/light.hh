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
    virtual Vector3 computeDir(Point3 p) const = 0;
};

class PointLight : public Light
{
public:
    Point3 center_;

    PointLight(double intensity, const Color color, const Point3 center);

    double computeIntensity(Vector3 light_dir) const override;
    Vector3 computeDir(Point3 p) const override;
};

class SunLight : public Light
{
public:
    double lati_;
    double longi_;
    Vector3 light_dir_;

    SunLight(double intensity, double lati, double longi);

    double computeIntensity(Vector3 light_dir) const override;
    Vector3 computeDir(Point3 p) const override;

    static double lati_to_spherical(double lati);
    static double longi_to_spherical(double longi);

    static double lati_to_temperature(double lati);

    static Color base_lati_to_color(double lati);
};

class AmbientLight
{
public:
    double intensity_;
    Color color_;

    AmbientLight(double intensity, const Color color);

    Color getAmbientLight() const;
};