#include "rendering.hh"

#include "thread_pool.hh"
#include "utils.hh"

void Rendering::render(Scene &scene, Image2D &image)
{
    unsigned int numThreads = std::thread::hardware_concurrency();
    std::cout << "Number of threads: " << numThreads << std::endl;
    ThreadPool pool(numThreads);

    for (int y = 0; y < image.height_; y++)
    {
        // One thread per row
        pool.enqueue([y, &scene, &image] {
            for (int x = 0; x < image.width_; x++)
            {
                Ray ray = scene.cam_.getRayAt(y, x);
                // std::cout << ray.origin_ << std::endl;
                auto pixel_color = castRay(ray, scene, 1);
                image.setPixel(y, x, pixel_color);
            }
        });
    }

    while (!pool.isQueueEmpty())
    {
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

Color Rendering::castRay(const Ray &ray, const Scene &scene, int iter)
{
    if (iter > max_iter)
        return Color(0.0, 0.0, 0.0);

    // std::cout << ray.direction_ << std::endl;

    HitRecord closest_hit_record;
    bool has_hit = getClosestObj(ray, scene.objects_, closest_hit_record);

    if (has_hit)
    {
        Point3 p = closest_hit_record.p;
        Vector3 n = closest_hit_record.n;
        LocalTexture loc_tex = closest_hit_record.tex;

        Color color = scene.ambient_light_->getAmbientLight();

        Vector3 reflect_ray_dir = Vector3::unit_vector(
            Vector3::reflect(Vector3::unit_vector(ray.direction_), n));

        for (auto const &light : scene.lights_)
        {
            // Consider shadows
            Vector3 light_dir = light->computeDir(p);
            double light_intensity = light->computeIntensity(light_dir);
            Ray light_dir_ray =
                Ray(p + (utils::kEpsilon * light_dir), light_dir);
            bool has_hit_light_dir = hasAnyObj(light_dir_ray, scene.objects_);
            if (has_hit_light_dir)
            {
                light_intensity = 0.0;
            }

            // Diffuse componant
            double dot_n_light = Vector3::dot(n, light_dir);
            if (dot_n_light > 0)
            {
                color += loc_tex.kd_ * loc_tex.color_ * dot_n_light
                    * light->color_ * light_intensity;
            }

            // Specular componant
            double dot_specular = Vector3::dot(reflect_ray_dir, light_dir);
            if (dot_specular > 0)
                color += loc_tex.ks_ * light_intensity * light->color_
                    * pow(dot_specular, loc_tex.ns_);
        }

        // Reflexion componant
        if (loc_tex.ks_ > 0)
        {
            Ray reflect_ray =
                Ray(p + (utils::kEpsilon * reflect_ray_dir), reflect_ray_dir);
            color += loc_tex.ks_ * castRay(reflect_ray, scene, iter + 1);
        }

        // Emission componant
        color += loc_tex.emission_ * loc_tex.color_;

        return color;
    }
    return scene.skybox_->getSkyboxAt(ray.direction_);
}

bool Rendering::getClosestObj(const Ray &ray,
                              const list<shared_ptr<PhysObj>> &objects,
                              HitRecord &hit_record)
{
    HitRecord closest_hit_record;
    closest_hit_record.t = utils::infinity;
    bool hit_anything = false;
    for (auto const &object : objects)
    {
        HitRecord object_hit_record;
        bool has_hit = object->hit(ray, object_hit_record);
        if (has_hit && object_hit_record.t < closest_hit_record.t)
        {
            closest_hit_record = object_hit_record;
            hit_anything = true;
        }
    }
    if (!hit_anything)
    {
        return false;
    }

    hit_record = closest_hit_record;
    return hit_anything;
}

bool Rendering::hasAnyObj(const Ray &ray,
                          const list<shared_ptr<PhysObj>> &objects)
{
    for (auto const &object : objects)
    {
        HitRecord object_hit_record;
        if (object->hit(ray, object_hit_record))
        {
            return true;
        }
    }
    return false;
}