#pragma once

namespace Math
{
    class Vec2
    {
    public:
        Vec2();
        Vec2(float x, float y);

        void setX(float x);
        void setY(float y);

        float x;
        float y;
    };
} // namespace Math