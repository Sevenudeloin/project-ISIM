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

Vector3 PhysObj::get_normal_at(double y, double x) const
{
    return mat_->get_normal_at(y, x);
}