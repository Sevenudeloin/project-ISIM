#include "interval.hh"

#include "utils.hh"

Interval::Interval()
    : min_(+utils::infinity)
    , max_(-utils::infinity)
{}

Interval::Interval(double min, double max)
    : min_(min)
    , max_(max)
{}

bool Interval::contains(double value) const
{
    return (min_ <= value) && (value <= max_);
}

bool Interval::surrounds(double value) const
{
    return (min_ < value) && (value < max_);
}

double Interval::clamp(double value) const
{
    if (value < min_)
        return min_;
    if (value > max_)
        return max_;
    return value;
}