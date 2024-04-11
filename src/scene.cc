#include "scene.hh"

#include <memory>

#include "heightmap.hh"
#include "ocean.hh"
#include "simplex_island_generator.hh"
#include "terrain.hh"
#include "terrain_texture.hh"
#include "triangle.hh"

Scene::Scene(Camera cam, list<shared_ptr<PhysObj>> objects,
             list<shared_ptr<Light>> lights, shared_ptr<SkyBox> skybox,
             shared_ptr<AmbientLight> ambient_light,
             shared_ptr<AbsorptionVolume> fog)
    : cam_(cam)
    , objects_(objects)
    , lights_(lights)
    , skybox_(skybox)
    , ambient_light_(ambient_light)
    , fog_(fog)
{}

Scene Scene::createTestScene(int image_height, int image_width)
{
    double sea_level = 0.2;
    double xy_scale = 1.0;
    double strength = 3.0;

    // FIXME : mettre toute la génération proc de heightmap dans une fonction
    float scale = 3.f;
    float offset_x = 5.9f;
    float offset_y = 15.5f;
    float offset_z = 0.0f;

    int heightmap_width = 30;

    float upscaling = 10.f;
    Heightmap base_heightmap = Heightmap(heightmap_width, heightmap_width);
    Heightmap upscaled_heightmap = Heightmap(heightmap_width * upscaling, heightmap_width * upscaling);

    SimplexIslandParameters params = SimplexIslandParameters(scale, offset_x, offset_y, offset_z, 1.0f);

    SimplexIslandGenerator simplexIslandGenerator = SimplexIslandGenerator();
    simplexIslandGenerator.generateHeightmaps(base_heightmap, upscaled_heightmap, params);

    // To preview the heightmaps
    Image2D base_img = Image2D(base_heightmap);
    Image2D upscaled_img = Image2D(upscaled_heightmap);
    base_img.writePPM("../images/heightmaps/base_flattened.ppm");
    upscaled_img.writePPM("../images/heightmaps/upscaled_flattened.ppm");

    // FIN DU CODE D EWAN

    auto full_heightmap = make_shared<Heightmap>(
        "../images/heightmaps/height_mountain_500x500.ppm");
    auto heightmap = make_shared<Heightmap>(
        "../images/heightmaps/height_mountain_40x40.ppm");

    auto ocean_normal_map =
        make_shared<Image2D>("../images/normalmaps/water_normal.ppm");

    auto terrain_tex_params = TerrainTextureParameters();
    auto terrain_tex = make_shared<TerrainTexture>(
        full_heightmap, sea_level, strength, xy_scale, terrain_tex_params, 3);

    Color ocean_color = Color::fromRGB(9, 22, 38, 0);
    auto ocean_tex = make_shared<OceanTexture>(
        LocalTexture(ocean_color, 1.0, 0.35, 2, 0.0,
                     make_shared<ExponentialAbsorptionVolume>(ocean_color, 40)),
        ocean_normal_map, Vector3(10.0, 3.0, 10.0));

    list<shared_ptr<PhysObj>> objs;

    auto terrain =
        Terrain::create_terrain(heightmap, xy_scale, strength, terrain_tex,
                                Vector3(-20, -(sea_level * strength), -43));

    auto ocean = make_shared<Ocean>(sea_level, ocean_tex);

    objs.push_back(terrain);
    objs.push_back(ocean);

    auto cloud_mask = make_shared<Image2D>("../images/cloudmaps/clouds_1.ppm");
    auto clouds_plan = make_shared<CloudsPlan>(cloud_mask, 20.0, 30.0, 1.0);

    list<shared_ptr<Light>> lights;
    auto sunlight = make_shared<SunLight>(2.0, 25.0, 0.0, clouds_plan);
    lights.push_back(sunlight);

    double aspect_ratio =
        static_cast<double>(image_width) / static_cast<double>(image_height);

    auto cam = Camera(Point3(0, 4, -3), Point3(0, 1, -10), Vector3(0, 1, 0),
                      85.0, 1.0, aspect_ratio, image_width);

    auto skybox = make_shared<SkyBoxImage>("../images/skyboxes/skybox_1.ppm");

    auto ambient_light =
        make_shared<AmbientLight>(0.05, Color::fromRGB(100, 100, 180));

    auto fog = make_shared<LinearAbsorptionVolume>(Color(0.6, 0.6, 0.6), 1.5,
                                                   5.0, 0.25);

    return Scene(cam, objs, lights, skybox, ambient_light, fog);
}

/*
Scene Scene::createOceanScene(int image_height, int image_width)
{
    auto uniform_ocean_tex = make_shared<OceanTexture>(
        LocalTexture(Color(0.1, 0.1, 0.9), 0.7, 0.3, 1),
        "../images/normalmaps/water_normal.ppm", Vector3(50.0, 2.0, 50.0));

    list<shared_ptr<PhysObj>> objs;

    auto ocean = make_shared<Ocean>(0.3, uniform_ocean_tex);

    objs.push_back(ocean);

    list<shared_ptr<Light>> lights;
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

    list<shared_ptr<Light>> lights;
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
*/