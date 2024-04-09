#include "terrain.hh"

#include <iostream>

Point3 Terrain::make_terrain_point_at(int y, int x, float height)
{
    return Point3(xy_scale_ * x, height_scale_ * height, xy_scale_ * y);
}

Terrain::Terrain(int height, int width, shared_ptr<TextureMaterial> mat)
    : PhysObj(mat)
    , height_(height)
    , width_(width)
    , xy_scale_(1)
    , height_scale_(1)
    , heightmap_(std::make_shared<Heightmap>(width, height))
    , mesh_(height,
            vector<SquareTriangle>(width, SquareTriangle(nullptr, nullptr)))
{}

Terrain::Terrain(shared_ptr<Heightmap> heightmap, float xy_scale,
                 float height_scale, shared_ptr<TextureMaterial> mat)
    : PhysObj(mat)
    , height_(heightmap->height_)
    , width_(heightmap->width_)
    , xy_scale_(xy_scale)
    , height_scale_(height_scale)
    , heightmap_(heightmap)
    , mesh_(heightmap->height_,
            vector<SquareTriangle>(heightmap->width_,
                                   SquareTriangle(nullptr, nullptr)))
{}

void Terrain::create_mesh()
{
    for (int y = 0; y < heightmap_->height_ - 1; y++)
    {
        for (int x = 0; x < heightmap_->width_ - 1; x++)
        {
            float top_left_corner_h = heightmap_->at(y, x);
            float top_right_corner_h = heightmap_->at(y, x + 1);
            float bot_left_corner_h = heightmap_->at(y + 1, x);
            float bot_right_corner_h = heightmap_->at(y + 1, x + 1);

            Point3 top_left_corner =
                make_terrain_point_at(y, x, top_left_corner_h);
            Point3 top_right_corner =
                make_terrain_point_at(y, x + 1, top_right_corner_h);
            Point3 bot_left_corner =
                make_terrain_point_at(y + 1, x, bot_left_corner_h);
            Point3 bot_right_corner =
                make_terrain_point_at(y + 1, x + 1, bot_right_corner_h);

            auto first_triangle = std::make_shared<Triangle>(
                top_left_corner, bot_left_corner, top_right_corner, mat_, this);
            auto second_triangle =
                std::make_shared<Triangle>(top_right_corner, bot_left_corner,
                                           bot_right_corner, mat_, this);

            mesh_[y][x].first = first_triangle;
            mesh_[y][x].second = second_triangle;
        }
    }
}

void Terrain::translate(const Vector3 &v)
{
    translation_ = translation_ + v;
}

LocalTexture Terrain::get_texture_at(const Point3 &p) const
{
    Point3 local_p = p - translation_;
    local_p.x_ /= xy_scale_ * width_;
    local_p.z_ /= xy_scale_ * height_;
    return mat_->get_texture_at(local_p);
}

Vector3 Terrain::get_normal_at(const Point3 &p) const
{
    // return Vector3(0, 1, 0);
    Point3 local_p = p - translation_;
    local_p.x_ /= xy_scale_ * width_;
    local_p.z_ /= xy_scale_ * height_;
    return mat_->get_normal_at(local_p);
}

bool Terrain::hit(const Ray &ray, HitRecord &hit_record) const
{
    // std ::cout << ray.origin_ << std::endl;
    HitRecord closest_hit_record;
    closest_hit_record.t = utils::infinity;
    bool hit_anything = false;

    for (int y = 0; y < height_ - 1; y++)
    {
        for (int x = 0; x < width_ - 1; x++)
        {
            auto triangle = mesh_[y][x].first;
            HitRecord triangle_hit_record;
            bool has_hit = triangle->hit(ray, triangle_hit_record);
            if (has_hit && triangle_hit_record.t < closest_hit_record.t)
            {
                closest_hit_record = triangle_hit_record;
                hit_anything = true;
            }

            triangle = mesh_[y][x].second;
            has_hit = triangle->hit(ray, triangle_hit_record);
            if (has_hit && triangle_hit_record.t < closest_hit_record.t)
            {
                closest_hit_record = triangle_hit_record;
                hit_anything = true;
            }
        }
    }

    if (!hit_anything)
    {
        return false;
    }

    hit_record = closest_hit_record;
    return true;
}

shared_ptr<Terrain> Terrain::create_terrain(shared_ptr<Heightmap> heightmap,
                                            float xy_scale, float height_scale,
                                            shared_ptr<TextureMaterial> mat,
                                            const Vector3 &v)
{
    auto terrain =
        std::make_shared<Terrain>(heightmap, xy_scale, height_scale, mat);
    terrain->create_mesh();
    terrain->translate(v);
    return terrain;
}