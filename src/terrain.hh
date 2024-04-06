#pragma once

#include <tuple>
#include <vector>

#include "heightmap.hh"
#include "physobj.hh"
#include "triangle.hh"
#include "utils.hh"

using std::shared_ptr;
using std::pair;
using std::vector;

using SquareTriangle = pair<shared_ptr<Triangle>, shared_ptr<Triangle>>;
using TriangleLine = vector<SquareTriangle>;
using Triangle2DMesh = vector<TriangleLine>;

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

    void create_mesh();

    bool hit(const Ray &ray, HitRecord &hit_record) const override;

    void translate(const Vector3 &v) override;

    Vector3 get_normal_at(const Point3 &p) const override;

    static shared_ptr<Terrain> create_terrain(shared_ptr<Heightmap> heightmap,
                                              float xy_scale,
                                              float height_scale,
                                              shared_ptr<TextureMaterial> mat,
                                              const Vector3 &v);
};