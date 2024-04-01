#pragma once

#include <tuple>
#include <vector>

#include "physobj.hh"
#include "triangle.hh"
#include "utils.hh"

using std::shared_ptr;
using std::pair;
using std::vector;

using SquareTriangle = pair<shared_ptr<Triangle>, shared_ptr<Triangle>>;
using TriangleLine = vector<SquareTriangle>;
using Triangle2DMesh = vector<TriangleLine>;

class Heightmap
{
private:
    int height_;
    int width_;

public:
    int height();
    int width();

    float at(int x, int y);
};

class Terrain : public PhysObj
{
private:
    int height_;
    int width_;
    float xy_scale_;
    float height_scale_;
    shared_ptr<Heightmap> heightmap_;
    Triangle2DMesh mesh_;

    Point3 make_terrain_point_at(int y, int x, float height);

public:
    Terrain();
    Terrain(int height, int width, shared_ptr<TextureMaterial> mat);
    Terrain(shared_ptr<Heightmap> Heightmap, float xy_scale, float height_scale,
            shared_ptr<TextureMaterial> mat);

    bool hit(const Vector3 &ray, HitRecord &hit_record) const override;
};