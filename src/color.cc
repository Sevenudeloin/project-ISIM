#include "color.hh"

#include <cmath>

Color::Color()
    : r_(0)
    , g_(0)
    , b_(0)
    , a_(1.0)
{}

Color::Color(double r, double g, double b, double a)
    : r_(r)
    , g_(g)
    , b_(b)
    , a_(a)
{}

Color Color::fromRGB(int r, int g, int b, int a)
{
    return Color(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
}

Color Color::operator+(const Color &color)
{
    return Color(r_ + color.r_, g_ + color.g_, b_ + color.b_, a_ + color.a_);
}

Color Color::operator+=(const Color &color)
{
    r_ += color.r_;
    g_ += color.g_;
    b_ += color.b_;
    a_ += color.a_;
    return *this;
}

Color Color::operator-(const Color &color)
{
    return Color(r_ - color.r_, g_ - color.g_, b_ - color.b_, a_ - color.a_);
}

Color Color::operator-=(const Color &color)
{
    r_ -= color.r_;
    g_ -= color.g_;
    b_ -= color.b_;
    a_ -= color.a_;
    return *this;
}

Color Color::operator*(double scalar)
{
    return Color(scalar * r_, scalar * g_, scalar * b_, scalar * a_);
}

Color Color::operator*(const Color &color)
{
    return Color(r_ * color.r_, g_ * color.g_, b_ * color.b_, a_ * color.a_);
}

double Color::linear_to_gamma(double linear_component)
{
    return sqrt(linear_component);
}

Color Color::clamp(const Interval &interval) const
{
    return Color(interval.clamp(r_), interval.clamp(g_), interval.clamp(b_),
                 interval.clamp(a_));
}

Color operator*(double scalar, const Color &color)
{
    return Color(scalar * color.r_, scalar * color.g_, scalar * color.b_,
                 scalar * color.a_);
}

Color Color::temperature_to_color(double temperature)
{
    double temp = temperature / 100.0;

    double red, green, blue;

    if (temp <= 66)
    {
        red = 255.0;
        green = temp;
        green = 99.4708025861 * std::log(green) - 161.1195681661;

        if (temp <= 19)
        {
            blue = 0;
        }
        else
        {
            blue = temp - 10.0;
            blue = 138.5177312231 * std::log(blue) - 305.0447927307;
        }
    }
    else
    {
        red = temp - 60.0;
        red = 329.698727446 * std::pow(red, -0.1332047592);

        green = temp - 60.0;
        green = 288.1221695283 * std::pow(green, -0.0755148492);

        blue = 255.0;
    }

    red = Interval(0.0, 1.0).clamp(red / 255.0);
    green = Interval(0.0, 1.0).clamp(green / 255.0);
    blue = Interval(0.0, 1.0).clamp(blue / 255.0);
    return Color(red, green, blue);
}