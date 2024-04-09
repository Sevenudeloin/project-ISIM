#include "terrain_texture_parameters.hh"

#include <cmath>
#include <iostream>

#include "utils.hh"

TerrainTextureParameters::TerrainTextureParameters(
    std::list<std::tuple<double, std::shared_ptr<TerrainLayerTexture>>>
        terrain_layers_textures,
    std::shared_ptr<TerrainLayerTexture> above_texture, double cliff_threshold,
    std::shared_ptr<TerrainLayerTexture> cliff_texture, double beach_height,
    std::shared_ptr<TerrainLayerTexture> beach_texture)
    : terrain_layers_textures_(terrain_layers_textures)
    , above_texture_(above_texture)
    , cliff_texture_(cliff_texture)
    , beach_height_(beach_height)
    , beach_texture_(beach_texture)
{
    cliff_threshold_ = std::sin(utils::degrees_to_radians(cliff_threshold));
}

TerrainTextureParameters::TerrainTextureParameters()
    : terrain_layers_textures_(
        std::list<std::tuple<double, std::shared_ptr<TerrainLayerTexture>>>())
    , above_texture_(std::make_shared<TerrainLayerTexture>(
          TerrainLayerTexture::snow_texture))
    , cliff_texture_(std::make_shared<TerrainLayerTexture>(
          TerrainLayerTexture::cliff_texture))
    , beach_height_(0.05)
    , beach_texture_(std::make_shared<TerrainLayerTexture>(
          TerrainLayerTexture::beach_texture))
{
    cliff_threshold_ = std::sin(utils::degrees_to_radians(70));

    // Add grass texture layer
    terrain_layers_textures_.push_back(
        std::make_tuple(0.8,
                        std::make_shared<TerrainLayerTexture>(
                            TerrainLayerTexture::grass_texture)));

    // Add rock texture layer
    terrain_layers_textures_.push_back(
        std::make_tuple(0.9,
                        std::make_shared<TerrainLayerTexture>(
                            TerrainLayerTexture::cliff_texture)));
}

LocalTexture TerrainTextureParameters::getTerrainTexture(Point3 p, Vector3 n,
                                                         double sea_level) const
{
    if (n.y_ < cliff_threshold_)
    {
        return cliff_texture_->get_texture_at(p);
    }
    if (p.y_ < sea_level + beach_height_)
    {
        return beach_texture_->get_texture_at(p);
    }
    for (auto it = terrain_layers_textures_.begin();
         it != terrain_layers_textures_.end(); ++it)
    {
        if (p.y_ < std::get<0>(*it))
        {
            return std::get<1>(*it)->get_texture_at(p);
        }
    }
    return above_texture_->get_texture_at(p);
}