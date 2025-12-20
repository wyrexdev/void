#pragma once

namespace Math
{
    class Vec3
    {
    public:
        Vec3();
        Vec3(float x, float y, float z);

        void setX(float x);
        void setY(float y);
        void setZ(float z);

        float x;
        float y;
        float z;
    };
} // namespace Math