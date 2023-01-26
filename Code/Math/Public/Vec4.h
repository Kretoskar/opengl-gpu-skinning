#pragma once

template <typename T>
struct TVec4
{
    union
    {
        struct
        {
            T x;
            T y;
            T z;
            T w;
        };

        T v[4];
    };

    TVec4<T>() : x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)), w(static_cast<T>(0))
    {
    }

    TVec4<T>(T _x, T _y, T _z, T _w) :
        x(_x), y(_y), z(_z), w(_w)
    {
    }

    TVec4<T>(T* fv) :
        x(fv[0]), y(fv[1]), z(fv[2]), w(fv[3])
    {
    }
};

using Vec4 = TVec4<float>;
using IVec4 = TVec4<int>;
using UIVec4 = TVec4<unsigned int>;
