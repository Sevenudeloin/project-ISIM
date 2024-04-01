#pragma once

#include "color.hh"
#include "point3.hh"

struct LocalTexture
{
    Color kd_;
    double ks_;
    double ns_;

    LocalTexture()
        : kd_(Color())
        , ks_(0)
        , ns_(0)
    {}

    LocalTexture(Color col, double kd, double ks, double ns)
        : kd_(col * kd)
        , ks_(ks)
        , ns_(ns)
    {}
};

class TextureMaterial
{
public:
    virtual ~TextureMaterial() = default;

    virtual LocalTexture get_texture_at(Point3 pos) const = 0;
};

class UniformTexture : public TextureMaterial
{
private:
    LocalTexture tex_;

public:
    UniformTexture(LocalTexture tex)
        : tex_(tex)
    {}

    LocalTexture get_texture_at(Point3 pos) const override
    {
        return tex_;
    }

    const static UniformTexture default_mat;
};

const static UniformTexture default_mat(LocalTexture(Color(0.5, 0.5, 0.5), 0.8,
                                                     0.1, 0.5));