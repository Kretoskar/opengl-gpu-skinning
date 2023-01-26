#pragma once

template <typename T>
struct TVec2
{
    union
    {
        struct
        {
            T x;
            T y;
        };

        T v[2];
    };

    TVec2() : x(T(0)), y(T(0))
    {
    }

    TVec2(T _x, T _y) :
        x(_x), y(_y)
    {
    }

    TVec2(T* fv) :
        x(fv[0]), y(fv[1])
    {
    }
};

using Vec2 = TVec2<float>;
using IVec2 = TVec2<int>;
