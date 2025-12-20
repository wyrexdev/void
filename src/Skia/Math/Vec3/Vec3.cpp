#include "Skia/Math/Vec3/Vec3.hpp"

namespace Math
{
    Vec3::Vec3(float x, float y, float z)
    {
        setX(x);
        setY(y);
        setZ(z);
    }

    Vec3::Vec3()
    {
    }

    void Vec3::setX(float x)
    {
        this->x = x;
    }

    void Vec3::setY(float y)
    {
        this->y = y;
    }

    void Vec3::setZ(float z)
    {
        this->z = z;
    }
} // namespace Math
