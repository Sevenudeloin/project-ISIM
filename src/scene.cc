#include "scene.hh"

#include "ocean.hh"
#include "simplex_noise.hh"
#include "terrain.hh"
#include "terrain_texture.hh"
#include "triangle.hh"
#include <memory>

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

    SimplexNoiseGenerator simplexNoiseGenerator = SimplexNoiseGenerator(scale, 0.5f, 1.99f, 0.5f);
    // SimplexNoiseGenerator simplexNoiseGenerator = SimplexNoiseGenerator(5 + std::log(scale), 0.1f/scale, 0.5f, 1.99f, 0.5f);
    Heightmap heightmap1 = simplexNoiseGenerator.generateHeightmap(heightmap_width, heightmap_width, scale, offset_x, offset_y, offset_z);
    float upscaling = 10.f;
    SimplexNoiseGenerator simplexNoiseGenerator2 = SimplexNoiseGenerator(scale * upscaling, 0.5f, 1.99f, 0.5f);
    Heightmap heightmap2 = simplexNoiseGenerator2.generateHeightmap(heightmap_width * upscaling, heightmap_width * upscaling, scale * upscaling, offset_x, offset_y, offset_z);

    // To preview the heightmaps
    Image2D heightmap_image = heightmap1.toImage2D();
    heightmap_image.writePPM("../images/heightmaps/heightmap_output.ppm");
    Image2D heightmap_image2 = heightmap2.toImage2D();
    heightmap_image2.writePPM("../images/heightmaps/heightmap_output2.ppm");

    // heightmap2.flattenSides(heightmap_width * upscaling / 42);

    auto heightmap_ptr = make_shared<Heightmap>(heightmap1);
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