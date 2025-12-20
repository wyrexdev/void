#include "Skia/Math/Vec2/Vec2.hpp"

namespace Math
{
    Vec2::Vec2(float x, float y)
    {
        setX(x);
        setY(y);
    }

    Vec2::Vec2()
    {
    }

    void Vec2::setX(float x)
    {
        this->x = x;
    }

    void Vec2::setY(float y)
    {
        this->y = y;
    }
} // namespace Math
