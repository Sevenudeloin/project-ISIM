#include "scene.hh"

#include "ocean.hh"
#include "terrain.hh"
#include "triangle.hh"

Scene::Scene(Camera cam, list<shared_ptr<PhysObj>> objects,
             list<shared_ptr<PointLight>> lights, shared_ptr<SkyBox> skybox)
    : cam_(cam)
    , objects_(objects)
    , lights_(lights)
    , skybox_(skybox)
{}

Scene Scene::createTestScene(int image_height, int image_width)
{
    auto uniform_terrain_tex = make_shared<UniformTexture>(
        LocalTexture(Color(0.1, 0.7, 0.1), 0.95, 0.02, 2));
    auto uniform_ocean_tex = make_shared<OceanTexture>(
        LocalTexture(Color(0.1, 0.1, 0.9), 0.7, 0.3, 1),
        "../images/normalmaps/water_normal.ppm", Vector3(50.0, 1.0, 50.0));

    list<shared_ptr<PhysObj>> objs;

    // auto heightmap = PerlinNoiseGenerator.generateHeightmap(100, 100);
    auto heightmap = make_shared<Heightmap>(
        "../images/heightmaps/heightmap_simple_20x20.ppm");
    auto terrain = make_shared<Terrain>(heightmap, 1, 2, uniform_terrain_tex);
    terrain->translate(Vector3(-9.75, 0, -20));

    auto ocean = make_shared<Ocean>(0.3, uniform_ocean_tex);

    objs.push_back(terrain);
    objs.push_back(ocean);

    list<shared_ptr<PointLight>> lights;
    auto light_one = make_shared<PointLight>(1.0, Color(1.0, 1.0, 1.0),
                                             Point3(5.0, 10.0, 0.0));
    lights.push_back(light_one);

    double aspect_ratio =
        static_cast<double>(image_width) / static_cast<double>(image_height);
    auto cam = Camera(Point3(0, 3, 5), Point3(0, 0, -10), Vector3(0, 1, 0),
                      90.0, 1.0, aspect_ratio, image_width);

    auto skybox = make_shared<SkyBoxImage>("../images/skyboxes/skybox_1.ppm");

    return Scene(cam, objs, lights, skybox);
}

Scene Scene::createOceanScene(int image_height, int image_width)
{
    auto uniform_ocean_tex = make_shared<OceanTexture>(
        LocalTexture(Color(0.1, 0.1, 0.9), 0.7, 0.3, 1),
        "../images/normalmaps/water_normal.ppm", Vector3(50.0, 2.0, 50.0));

    list<shared_ptr<PhysObj>> objs;

    auto ocean = make_shared<Ocean>(0.3, uniform_ocean_tex);

    objs.push_back(ocean);

    list<shared_ptr<PointLight>> lights;
    auto light_one = make_shared<PointLight>(1.0, Color(1.0, 1.0, 1.0),
                                             Point3(0.0, 10.0, 0.0));
    lights.push_back(light_one);

    double aspect_ratio =
        static_cast<double>(image_width) / static_cast<double>(image_height);
    auto cam = Camera(Point3(0, 3, 5), Point3(0, 0, -10), Vector3(0, 1, 0),
                      90.0, 1.0, aspect_ratio, image_width);

    auto skybox = make_shared<SkyBoxImage>("../images/skyboxes/skybox_1.ppm");

    return Scene(cam, objs, lights, skybox);
}

Scene Scene::createIslandScene(int image_height, int image_width)
{
    auto uniform_terrain_tex = make_shared<UniformTexture>(
        LocalTexture(Color(0.1, 0.7, 0.1), 0.95, 0.02, 2));
    auto uniform_ocean_tex = make_shared<OceanTexture>(
        LocalTexture(Color(0.1, 0.1, 0.9), 0.7, 0.3, 1),
        "../images/normalmaps/water_normal.ppm", Vector3(50.0, 2.0, 50.0));

    list<shared_ptr<PhysObj>> objs;

    auto heightmap = make_shared<Heightmap>(
        "../images/heightmaps/heightmap_simple_20x20.ppm");
    auto terrain = make_shared<Terrain>(heightmap, 1, 2, uniform_terrain_tex);
    terrain->translate(Vector3(-9.75, 0, -20));

    auto ocean = make_shared<Ocean>(0.3, uniform_ocean_tex);

    objs.push_back(terrain);
    objs.push_back(ocean);

    list<shared_ptr<PointLight>> lights;
    auto light_one = make_shared<PointLight>(1.0, Color(1.0, 1.0, 1.0),
                                             Point3(0.0, 10.0, 0.0));
    lights.push_back(light_one);

    double aspect_ratio =
        static_cast<double>(image_width) / static_cast<double>(image_height);
    auto cam = Camera(Point3(0, 3, 5), Point3(0, 0, -10), Vector3(0, 1, 0),
                      90.0, 1.0, aspect_ratio, image_width);

    auto skybox = make_shared<SkyBoxImage>("../images/skyboxes/skybox_1.ppm");

    return Scene(cam, objs, lights, skybox);
}