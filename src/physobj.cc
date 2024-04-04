#include "physobj.hh"

PhysObj::PhysObj()
    : mat_(std::make_shared<UniformTexture>(default_mat))
{}

PhysObj::PhysObj(std::shared_ptr<TextureMaterial> mat)
    : mat_(mat)
{}

LocalTexture PhysObj::get_texture_at(const Point3 &p) const
{
    return mat_->get_texture_at(p);
}

Vector3 PhysObj::get_normal_at_local(float y, float x) const
{
    return mat_->get_normal_at_local(y, x);
}

Vector3 PhysObj::get_normal_at_global(const Point3 &p) const
{
    return mat_->get_normal_at_global(p);
}