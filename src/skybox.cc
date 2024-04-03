#include "skybox.hh"

SkyBoxGradient::SkyBoxGradient(Color up_color, Color horizon_color,
                               Color down_color)
    : up_color_(up_color)
    , horizon_color_(horizon_color)
    , down_color_(down_color)
{}

Color SkyBoxGradient::getSkyboxAt(Vector3 dir) const
{
    Vector3 unit_dir = Vector3::unit_vector(dir);

    double a = unit_dir.y_;
    if (a >= 0)
    {
        return (1.0 - a) * horizon_color_ + a * up_color_;
    }
    a *= -1;
    return (1.0 - a) * horizon_color_ + a * down_color_;
}

SkyBoxImage::SkyBoxImage(const std::string &filename)
{
    PPMParser parser(filename);

    parser.parse(pixels_, width_, height_);
}

Color SkyBoxImage::getSkyboxAt(Vector3 dir) const
{
    Vector3 unit_dir = Vector3::unit_vector(dir);

    // To spherical coordinate system
    double x = unit_dir.x_;
    double y = unit_dir.z_;
    double z = unit_dir.y_;

    double rho = unit_dir.length();
    double lati = std::acos(z / rho);
    double longi = std::acos(x / std::sqrt(x * x + y * y));
    if (y < 0.0)
        longi = 2 * utils::pi - longi;

    // To image coordinates
    int img_y = (lati / utils::pi) * static_cast<float>(height_);
    int img_x = (longi / (2.0 * utils::pi)) * static_cast<float>(width_);

    return pixels_[img_y * width_ + img_x];
}