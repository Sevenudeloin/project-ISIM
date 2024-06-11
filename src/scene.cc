#include "scene.hh"

#include <memory>

#include "dla_generator.hh"
#include "heightmap.hh"
#include "ocean.hh"
#include "ocean_texture.hh"
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
    double strength = 4.0;

    auto full_heightmap = make_shared<Heightmap>(
        "../images/heightmaps/height_mountain_500x500.ppm");
    auto heightmap = make_shared<Heightmap>(
        "../images/heightmaps/height_mountain_40x40.ppm");

    auto ocean_normal_map =
        make_shared<Image2D>("../images/normalmaps/water_normal.ppm");

    auto terrain_tex_params = TerrainTextureParameters();
    auto terrain_tex = make_shared<TerrainTexture>(
        full_heightmap, sea_level, strength, xy_scale, terrain_tex_params, 3);

    list<shared_ptr<PhysObj>> objs;

    auto terrain =
        Terrain::create_terrain(heightmap, xy_scale, strength, terrain_tex,
                                Vector3(-20, -(sea_level * strength), -43));

    Color ocean_color = Color::fromRGB(9, 22, 38, 0);
    auto ocean_tex = make_shared<OceanTexture>(
        LocalTexture(
            ocean_color, 1.0, 0.35, 2, 0.0,
            make_shared<ExponentialAbsorptionVolume>(ocean_color, 3, 1.5)),
        ocean_normal_map, terrain, sea_level, Vector3(10.0, 3.0, 10.0));

    auto ocean = make_shared<Ocean>(0, ocean_tex);

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

Scene Scene::createSimplexScene(int image_height, int image_width)
{
    double sea_level = 0.5;
    double xy_scale = 1.0;
    double strength = 6.5;

    float scale = 2.f;
    float offset_x = 5.9f;
    float offset_y = 15.5f;
    float offset_z = 0.0f;

    int heightmap_width = 50;

    float upscaling = 10.f;
    Heightmap base_heightmap = Heightmap(heightmap_width, heightmap_width);
    Heightmap upscaled_heightmap =
        Heightmap(heightmap_width * upscaling, heightmap_width * upscaling);

    SimplexIslandParameters params =
        SimplexIslandParameters(scale, offset_x, offset_y, offset_z, 1.0f);

    SimplexIslandGenerator simplexIslandGenerator = SimplexIslandGenerator();
    simplexIslandGenerator.generateHeightmaps(base_heightmap,
                                              upscaled_heightmap, params);

    // To preview the heightmaps
    Image2D base_img = Image2D(base_heightmap);
    Image2D upscaled_img = Image2D(upscaled_heightmap);
    base_img.writePPM("../images/heightmaps/base_flattened.ppm");
    upscaled_img.writePPM("../images/heightmaps/upscaled_flattened.ppm");

    // FIN DU CODE D EWAN

    auto full_heightmap = std::make_shared<Heightmap>(upscaled_heightmap);
    auto heightmap = std::make_shared<Heightmap>(base_heightmap);

    auto ocean_normal_map =
        make_shared<Image2D>("../images/normalmaps/water_normal.ppm");

    auto terrain_tex_params = TerrainTextureParameters();
    auto terrain_tex = make_shared<TerrainTexture>(
        full_heightmap, sea_level, strength, xy_scale, terrain_tex_params, 3);

    list<shared_ptr<PhysObj>> objs;

    auto terrain =
        Terrain::create_terrain(heightmap, xy_scale, strength, terrain_tex,
                                Vector3(-20, -(sea_level * strength), -43));

    Color ocean_color = Color::fromRGB(9, 22, 38, 0);
    auto ocean_tex = make_shared<OceanTexture>(
        LocalTexture(
            ocean_color, 1.0, 0.35, 2, 0.0,
            make_shared<ExponentialAbsorptionVolume>(ocean_color, 3, 1.5)),
        ocean_normal_map, terrain, sea_level, Vector3(10.0, 3.0, 10.0));

    auto ocean = make_shared<Ocean>(0, ocean_tex);

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

Scene Scene::createDLAScene(int image_height, int image_width)
{
    double sea_level = 0.1;
    double xy_scale = 0.325; // 1.3 for 32x32 mesh, 0.65 for 64x64 mesh, 0.325 for 128x128 mesh
    double strength = 6.5;

    // DLA::DLAGenerator generator = DLA::DLAGenerator(0.6, 0.5, 0.5, 10); // center of the graph is at 0.75, 0.75

    // int upscaled_width = 1024;
    // Heightmap upscaled_heightmap(upscaled_width, upscaled_width);
    // int base_width = 64;
    // Heightmap base_heightmap(base_width, base_width);

    // generator.generateHeightmaps(base_heightmap, upscaled_heightmap);

    // FIXME remove this if need demo load already computed DLA heightmap
    Heightmap upscaled_heightmap = Heightmap::readFromFile("../images/heightmaps/DLA_upscaled_flattened_2048_1.hmap");

    Heightmap base_heightmap = Heightmap::readFromFile("../images/heightmaps/DLA_base_flattened_128_1.hmap");

    // multiply every value from heightmaps by some factor and clamp them between 0 and 1
    double multiply_factor = 1.5;

    for (int i = 0; i < upscaled_heightmap.height_; i++)
    {
        for (int j = 0; j < upscaled_heightmap.width_; j++)
        {
            upscaled_heightmap.set(i, j, upscaled_heightmap.at(i, j) * multiply_factor);
        }
    }
    upscaled_heightmap.minMaxNormalize();

    for (int i = 0; i < base_heightmap.height_; i++)
    {
        for (int j = 0; j < base_heightmap.width_; j++)
        {
            base_heightmap.set(i, j, base_heightmap.at(i, j) * multiply_factor);
        }
    }
    base_heightmap.minMaxNormalize();

    // To preview the heightmaps
    Image2D base_img = Image2D(base_heightmap);
    // base_img.minMaxNormalize();
    base_img.writePPM("../images/heightmaps/base_flattened.ppm");

    Image2D upscaled_img = Image2D(upscaled_heightmap);
    // upscaled_img.minMaxNormalize();
    upscaled_img.writePPM("../images/heightmaps/upscaled_flattened.ppm");

    std::cout << "DLA heightmaps loaded\n"; // FIXME remove

    auto full_heightmap = std::make_shared<Heightmap>(upscaled_heightmap);
    auto heightmap = std::make_shared<Heightmap>(base_heightmap);

    auto ocean_normal_map =
        make_shared<Image2D>("../images/normalmaps/water_normal.ppm");

    auto terrain_tex_params = TerrainTextureParameters();
    auto terrain_tex = make_shared<TerrainTexture>(
        full_heightmap, sea_level, strength, xy_scale, terrain_tex_params, 3);

    list<shared_ptr<PhysObj>> objs;

    auto terrain =
        Terrain::create_terrain(heightmap, xy_scale, strength, terrain_tex,
                                Vector3(-20, -(sea_level * strength), -43));

    Color ocean_color = Color::fromRGB(9, 22, 38, 0);
    auto ocean_tex = make_shared<OceanTexture>(
        LocalTexture(
            ocean_color, 1.0, 0.35, 2, 0.0,
            make_shared<ExponentialAbsorptionVolume>(ocean_color, 7.0, 1.5)),
        ocean_normal_map, terrain, sea_level, Vector3(10.0, 3.0, 10.0));

    auto ocean = make_shared<Ocean>(0, ocean_tex);

    objs.push_back(terrain);
    objs.push_back(ocean);

    auto cloud_mask = make_shared<Image2D>("../images/cloudmaps/clouds_1.ppm");
    auto clouds_plan = make_shared<CloudsPlan>(cloud_mask, 20.0, 30.0, 1.0);

    list<shared_ptr<Light>> lights;
    auto sunlight = make_shared<SunLight>(2.0, 25.0, 0.0, clouds_plan);
    lights.push_back(sunlight);

    double aspect_ratio =
        static_cast<double>(image_width) / static_cast<double>(image_height);

    auto cam = Camera(Point3(0, 4.5, -4), Point3(0, 2, -8), Vector3(0, 1, 0),
                      90.0, 1.0, aspect_ratio, image_width);

    auto skybox = make_shared<SkyBoxImage>("../images/skyboxes/skybox_1.ppm");

    auto ambient_light =
        make_shared<AmbientLight>(0.05, Color::fromRGB(100, 100, 180));

    auto fog = make_shared<LinearAbsorptionVolume>(Color(0.6, 0.6, 0.6), 3,
                                                   7.0, 1.0);

    return Scene(cam, objs, lights, skybox, ambient_light, fog);
}