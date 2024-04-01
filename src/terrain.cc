#include "terrain.hh"

Point3 Terrain::make_terrain_point_at(int x, int y, float height)
{
    return Point3(xy_scale_ * x, xy_scale_ * y, height_scale_ * height);
}

Terrain::Terrain(int height, int width, shared_ptr<TextureMaterial> mat)
    : PhysObj(mat)
    , height_(height)
    , width_(width)
    , xy_scale_(1)
    , height_scale_(1)
{}

Terrain::Terrain(shared_ptr<Heightmap> heightmap, float xy_scale,
                 float height_scale, shared_ptr<TextureMaterial> mat)
    : PhysObj(mat)
    , height_(heightmap->height())
    , width_(heightmap->width())
    , xy_scale_(xy_scale)
    , height_scale_(height_scale)
    , heightmap_(heightmap)
    , mesh_(heightmap->height(),
            vector<SquareTriangle>(heightmap->width(),
                                   SquareTriangle(nullptr, nullptr)))
{
    for (int y = 0; y < heightmap->height() - 1; y++)
    {
        for (int x = 0; x < heightmap->width() - 1; x++)
        {
            float top_left_corner_h = heightmap->at(x, y);
            float top_right_corner_h = heightmap->at(x + 1, y);
            float bot_left_corner_h = heightmap->at(x, y + 1);
            float bot_right_corner_h = heightmap->at(x + 1, y + 1);

            Point3 top_left_corner =
                make_terrain_point_at(x, y, top_left_corner_h);
            Point3 top_right_corner =
                make_terrain_point_at(x + 1, y, top_right_corner_h);
            Point3 bot_left_corner =
                make_terrain_point_at(x, y + 1, bot_left_corner_h);
            Point3 bot_right_corner =
                make_terrain_point_at(x + 1, y + 1, bot_right_corner_h);

            auto first_triangle = std::make_shared<Triangle>(
                top_left_corner, top_right_corner, bot_left_corner);
            auto second_triangle = std::make_shared<Triangle>(
                top_right_corner, bot_right_corner, bot_left_corner);

            mesh_[y][x].first = first_triangle;
            mesh_[y][x].second = second_triangle;
        }
    }
}

bool Terrain::hit(const Vector3 &ray, HitRecord &hit_record) const
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

    hit_record.t = closest_hit_record.t;
    hit_record.p = closest_hit_record.p;
    hit_record.n = closest_hit_record.n;
    hit_record.tex = closest_hit_record.tex;
    return true;
}