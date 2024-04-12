#pragma once

#include <list>
#include <memory>

#include "absorption_volume.hh"
#include "camera.hh"
#include "image2d.hh"
#include "light.hh"
#include "physobj.hh"
#include "skybox.hh"

using std::list;
using std::make_shared;
using std::shared_ptr;

class Scene
{
public:
    Camera cam_;
    list<shared_ptr<PhysObj>> objects_;
    list<shared_ptr<Light>> lights_;
    shared_ptr<SkyBox> skybox_;
    shared_ptr<AmbientLight> ambient_light_;
    shared_ptr<AbsorptionVolume> fog_;

    Scene(Camera cam, list<shared_ptr<PhysObj>> objects,
          list<shared_ptr<Light>> lights, shared_ptr<SkyBox> skybox = nullptr,
          shared_ptr<AmbientLight> ambient_light = nullptr,
          shared_ptr<AbsorptionVolume> fog = nullptr);

    static Scene createTestScene(int image_height, int image_width);
    static Scene createSimplexScene(int image_height, int image_width);
};