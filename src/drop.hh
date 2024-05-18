#include <cmath>

struct Vector2
{
    float x, y;

    Vector2 operator+(const Vector2 &other) const
    {
        return { x + other.x, y + other.y };
    }

    Vector2 operator-(const Vector2 &other) const
    {
        return { x - other.x, y - other.y };
    }

    Vector2 operator*(float scalar) const
    {
        return { x * scalar, y * scalar };
    }

    Vector2 normalized() const
    {
        float length = std::sqrt(x * x + y * y);
        return { x / length, y / length };
    }
};

struct Drop
{
    Vector2 pos;
    Vector2 dir;
    float vel;
    float water;
    float sediment;
};