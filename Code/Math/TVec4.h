#pragma once

template<typename T>
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

    TVec4<T>() : x(T(0)), y(T(0)), z(T(0)), w(T(0)) {}
    TVec4<T>(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    TVec4<T>(T* fv) : x(fv[0]), y(fv[1]), z(fv[2]), w(fv[3]) {}
};

typedef TVec4<float> Vec4;
typedef TVec4<int> IVec4;
typedef TVec4<unsigned int> UIVec4;