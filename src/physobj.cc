#include "physobj.hh"

PhysObj::PhysObj()
    : mat_(std::make_shared<UniformTexture>(default_mat))
    , translation_(Vector3(0, 0, 0))
{}

PhysObj::PhysObj(std::shared_ptr<TextureMaterial> mat)
    : mat_(mat)
    , translation_(Vector3(0, 0, 0))
{}

LocalTexture PhysObj::get_texture_at(const Point3 &p) const
{
    return mat_->get_texture_at(p);
}

Vector3 PhysObj::get_normal_at(const Point3 &p) const
{
    return Vector3::unit_vector(mat_->get_normal_at(p));
}