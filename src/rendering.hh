#pragma once

#include "image2d.hh"
#include "scene.hh"

using std::shared_ptr;
using std::list;

class Rendering
{
public:
    static constexpr int max_iter = 2;

    static void render(Scene &scene, Image2D &image);

    static Color castRay(const Ray &ray, const Scene &scene, int iter,
                         double absorption = 0.0,
                         shared_ptr<Color> absorption_color = nullptr);

    static bool getClosestObj(const Ray &ray,
                              const list<shared_ptr<PhysObj>> &objects,
                              HitRecord &hit_record);

    static bool hasAnyObj(const Ray &ray,
                          const list<shared_ptr<PhysObj>> &objects);
};