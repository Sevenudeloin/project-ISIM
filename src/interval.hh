#pragma once

class Interval
{
public:
    double min_;
    double max_;

    Interval();
    Interval(double min, double max);

    bool contains(double value) const; // check if value is in the interval
    bool surrounds(double value)
        const; // check if value is in the interval but not on the edges

    double clamp(double value) const; // clamp value to the interval
};