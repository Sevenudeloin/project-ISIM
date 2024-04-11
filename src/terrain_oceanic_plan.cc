#include "terrain_oceanic_plan.hh"

#include <cmath>

#include "utils.hh"

TerrainOceanicPlan::TerrainOceanicPlan(double height,
                                       std::shared_ptr<TerrainLayerTexture> mat)
    : PhysObj(mat)
    , height_(height)
    , n_(0, 1, 0)
{}

bool TerrainOceanicPlan::hit(const Ray &ray, HitRecord &hit_record) const
{
    double nDotRayDir = Vector3::dot(n_, ray.direction_);
    if (Interval(-utils::kEpsilon, +utils::kEpsilon).surrounds(nDotRayDir))
    {
        // Ray perpendicular to the plane
        return false;
    }

    double d = -Vector3::dot(n_, Point3(0, height_, 0));
    double t = -(Vector3::dot(n_, ray.origin_) + d) / nDotRayDir;
    if (t < 0)
    {
        return false;
    }

    Point3 p = ray.at(t);

    hit_record.t = t;
    hit_record.p = p;
    hit_record.n = get_normal_at(p);
    hit_record.tex = LocalTexture(); // get_texture_at(p);
    return true;
}

void TerrainOceanicPlan::translate(const Vector3 &v)
{
    height_ += v.y_;
}

Point3 TerrainOceanicPlan::get_uv(const Point3 &p) const
{
    double x = p.x_;
    double y = p.z_;

    TerrainLayerTexture *real_mat_ =
        dynamic_cast<TerrainLayerTexture *>(mat_.get());
    double scale_x = real_mat_->scale_.x_;
    double scale_z = real_mat_->scale_.z_;

    if (scale_x == 0 || scale_z == 0)
        return Vector3(0, 0, 0);
    x = (x / scale_x) - std::floor(x / scale_x);
    y = (y / scale_z) - std::floor(y / scale_z);

    return Point3(x, p.y_, y);
}

LocalTexture TerrainOceanicPlan::get_texture_at(const Point3 &p) const
{
    Point3 uv = get_uv(p);
    return mat_->get_texture_at(Vector3(uv.x_, p.y_, uv.z_));
}

Vector3 TerrainOceanicPlan::get_normal_at(const Point3 &) const
{
    return n_;
}