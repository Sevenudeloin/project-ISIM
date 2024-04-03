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
    , mesh_(height,
            vector<SquareTriangle>(width, SquareTriangle(nullptr, nullptr)))
{
    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width - 1; x++)
        {
            Point3 top_left_corner = make_terrain_point_at(y, x, 0);
            Point3 top_right_corner = make_terrain_point_at(y, x + 1, 0);
            Point3 bot_left_corner = make_terrain_point_at(y + 1, x, 0);
            Point3 bot_right_corner = make_terrain_point_at(y + 1, x + 1, 0);

            auto first_triangle = std::make_shared<Triangle>(
                top_left_corner, bot_left_corner, top_right_corner, mat);
            auto second_triangle = std::make_shared<Triangle>(
                top_right_corner, bot_left_corner, bot_right_corner, mat);

            mesh_[y][x].first = first_triangle;
            mesh_[y][x].second = second_triangle;
        }
    }
}

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
{
    for (int y = 0; y < heightmap->height_ - 1; y++)
    {
        for (int x = 0; x < heightmap->width_ - 1; x++)
        {
            float top_left_corner_h = heightmap->at(y, x);
            float top_right_corner_h = heightmap->at(y, x + 1);
            float bot_left_corner_h = heightmap->at(y + 1, x);
            float bot_right_corner_h = heightmap->at(y + 1, x + 1);

            Point3 top_left_corner =
                make_terrain_point_at(y, x, top_left_corner_h);
            Point3 top_right_corner =
                make_terrain_point_at(y, x + 1, top_right_corner_h);
            Point3 bot_left_corner =
                make_terrain_point_at(y + 1, x, bot_left_corner_h);
            Point3 bot_right_corner =
                make_terrain_point_at(y + 1, x + 1, bot_right_corner_h);

            auto first_triangle = std::make_shared<Triangle>(
                top_left_corner, bot_left_corner, top_right_corner, mat);
            auto second_triangle = std::make_shared<Triangle>(
                top_right_corner, bot_left_corner, bot_right_corner, mat);

            mesh_[y][x].first = first_triangle;
            mesh_[y][x].second = second_triangle;
        }
    }
}

void Terrain::translate(const Vector3 &v)
{
    for (int y = 0; y < height_ - 1; y++)
    {
        for (int x = 0; x < width_ - 1; x++)
        {
            auto triangle = mesh_[y][x].first;
            triangle->translate(v);

            triangle = mesh_[y][x].second;
            triangle->translate(v);
        }
    }
}

bool Terrain::hit(const Ray &ray, HitRecord &hit_record) const
{
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