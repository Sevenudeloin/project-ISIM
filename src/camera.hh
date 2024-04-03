#pragma once

#include "ray.hh"
#include "vector3.hh"

class Camera
{
public:
    // Camera parameters
    Point3 center_;
    Point3 point_; // point the camera is looking at
    Vector3 up_; // up direction (y is height ?)
    double vfov_; // angle in y
    double zmin_; // image plan position

    // other Camera parameters
    Point3 pixel00_loc_;
    Vector3 viewport_u_;
    Vector3 viewport_v_;
    Point3 viewport_upper_left_;
    Vector3 pixel_delta_u_;
    Vector3 pixel_delta_v_;

    Camera(const Point3 &center, const Point3 &point, const Vector3 &up,
           double vfov, double zmin, double aspect_ratio, int image_width);

    Ray getRayAt(int y, int x) const;
};