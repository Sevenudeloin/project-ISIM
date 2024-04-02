#include "camera.hh"

#include <cmath>

#include "utils.hh"

Camera::Camera(const Point3& center, const Point3& point, const Vector3& up, double vfov, double zmin, double aspect_ratio, int image_width)
    : center_(center)
    , point_(point)
    , up_(up)
    , vfov_(vfov)
    , zmin_(zmin)
{
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    auto focal_length = (center_ - point_).length();

    auto vfov_radians = utils::degrees_to_radians(vfov_);
    auto h = std::tan(vfov_radians/2);
    auto viewport_height = 2 * h * focal_length;
    auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

    // Calculate u,v,w unit basis vector for the camera coord frame
    Vector3 w = Vector3::unit_vector(center_ - point_);
    Vector3 u = Vector3::unit_vector(Vector3::cross(up_, w));
    Vector3 v = Vector3::cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    viewport_u_ = viewport_width * u;
    viewport_v_ = viewport_height * -v;

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u_ = viewport_u_ / image_width;
    pixel_delta_v_ = viewport_v_ / image_height;

    // Calculate the location of the upper left pixel.
    // (we do minuses because the viewport is in the negative z axis)
    viewport_upper_left_ = center_ - (focal_length * w) - viewport_u_/2 - viewport_v_/2;
    pixel00_loc_ = viewport_upper_left_ + 0.5 * (pixel_delta_u_ + pixel_delta_v_);
}