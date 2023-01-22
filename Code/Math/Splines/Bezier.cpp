#include "Bezier.h"


template <typename T>
T Bezier<T>::Interpolate(float t)
{
    // readable version
    /*
    T A = lerp(curve.P1, curve.C1, t);
    T B = lerp(curve.C2, curve.P2, t);
    T C = lerp(curve.C1, curve.C2, t);
    T D = lerp(A, C, t);
    T E = lerp(C, B, t);
    T R = lerp(D, E, t);

    return R;
    */

    //optimised version
    return  P1 * ((1 - t) * (1 - t) * (1 - t)) +
            C1 * (3.0f * ((1 - t) * (1 - t)) * t) +
            C2 * (3.0f * (1 - t) * (t * t)) +
            P2 * (t * t * t);
}

template <typename T>
void Bezier<T>::Draw(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4)
{
    Vec3 red = Vec3(1, 0, 0);
    Vec3 green = Vec3(0, 1, 0);
    Vec3 blue = Vec3(0, 0, 1);
    Vec3 magenta = Vec3(1, 0, 1);
}
