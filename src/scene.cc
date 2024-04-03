#include "scene.hh"

Scene::Scene(Camera cam, list<shared_ptr<PhysObj>> objects,
             list<shared_ptr<PointLight>> lights, shared_ptr<SkyBox> skybox)
    : cam_(cam)
    , objects_(objects)
    , lights_(lights)
    , skybox_(skybox)
{}

Scene Scene::createTestScene(int image_height, int image_width)
{
    auto uniform_red_tex = make_shared<UniformTexture>(
        LocalTexture(Color(1.0, 0.0, 0.0), 0.8, 0.1, 0.9));

    list<shared_ptr<PhysObj>> objs;

    // auto heightmap = PerlinNoiseGenerator.generateHeightmap(100, 100);
    // auto terrain = make_shared<Terrain>(heightmap, 1, 1, uniform_red_tex);

    auto terrain = make_shared<Terrain>(10, 10, uniform_red_tex);
    objs.push_back(terrain);

    list<shared_ptr<PointLight>> lights;
    auto light_one = make_shared<PointLight>(1.0, Color(1.0, 1.0, 1.0),
                                             Point3(0.0, 3.0, 0.0));
    lights.push_back(light_one);

    double aspect_ratio =
        static_cast<double>(image_width) / static_cast<double>(image_height);
    auto cam = Camera(Point3(0, 1, 3), Point3(0, 0, -1), Vector3(0, 1, 0),
                      100.0, 1.0, aspect_ratio, image_width);

    auto skybox = make_shared<SkyBoxImage>("../images/skyboxes/skybox_1.ppm");

    return Scene(cam, objs, lights, skybox);
}