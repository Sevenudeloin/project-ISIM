#include "color.hh"

#include <cmath>

Color::Color()
    : r_(0), g_(0), b_(0)
{}

Color::Color(double r, double g, double b)
    : r_(r), g_(g), b_(b)
{}

Color Color::operator+(const Color& color)
{
    return Color(r_ + color.r_, g_ + color.g_, b_ + color.b_);
}

Color Color::operator+=(const Color& color)
{
    r_ += color.r_;
    g_ += color.g_;
    b_ += color.b_;
    return *this;
}

Color Color::operator-(const Color& color)
{
    return Color(r_ - color.r_, g_ - color.g_, b_ - color.b_);
}

Color Color::operator-=(const Color& color)
{
    r_ -= color.r_;
    g_ -= color.g_;
    b_ -= color.b_;
    return *this;
}

Color Color::operator*(double scalar)
{
    return Color(scalar * r_, scalar * g_, scalar * b_);
}

Color Color::operator*(const Color& color)
{
    return Color(r_ * color.r_, g_ * color.g_, b_ * color.b_);
}

double Color::linear_to_gamma(double linear_component)
{
    return sqrt(linear_component);
}

Color Color::clamp(const Interval& interval) const
{
    return Color(interval.clamp(r_), interval.clamp(g_), interval.clamp(b_));
}

Color operator*(double scalar, const Color& color)
{
    return Color(scalar * color.r_, scalar * color.g_, scalar * color.b_);
}