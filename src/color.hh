#pragma once

#include <ostream>

#include "interval.hh"

struct Color
{
    // values between 0.0 and 1.0
    double r_;
    double g_;
    double b_;

    Color();
    Color(double r, double g, double b);

    // values between 0 and 255 converted to [0, 1]
    static Color fromRGB(int r, int g, int b);

    Color operator+(const Color &color);
    Color operator+=(const Color &color);
    Color operator-(const Color &color);
    Color operator-=(const Color &color);
    Color operator*(double scalar);
    Color operator*(const Color &color);

    static double linear_to_gamma(
        double linear_component); // gamma correction is 2 (square root)
    static Color temperature_to_color(double temperature);

    Color clamp(const Interval &interval) const;

    friend Color operator*(double scalar, const Color &color);

    friend std::ostream &operator<<(std::ostream &os, const Color &color);
};

inline std::ostream &operator<<(std::ostream &os, const Color &color)
{
    os << "Color(" << color.r_ << ", " << color.g_ << ", " << color.b_ << ")";
    return os;
}