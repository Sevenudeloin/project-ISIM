#pragma once

#include <list>
#include <memory>

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
    list<shared_ptr<PointLight>> lights_;
    shared_ptr<SkyBox> skybox_;

    Scene(Camera cam, list<shared_ptr<PhysObj>> objects,
          list<shared_ptr<PointLight>> lights, shared_ptr<SkyBox> skybox);

    static Scene createTestScene(int image_height, int image_width);
    static Scene createOceanScene(int image_height, int image_width);
    static Scene createIslandScene(int image_height, int image_width);
};