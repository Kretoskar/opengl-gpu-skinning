#include "Bezier.h"

template <typename T>
T Bezier<T>::Evaluate(float t)
{
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
T Bezier<T>::Evaluate(T p1, T c1, T p2, T c2, float t)
{
    return  p1 * ((1 - t) * (1 - t) * (1 - t)) +
            c1 * (3.0f * ((1 - t) * (1 - t)) * t) +
            c2 * (3.0f * (1 - t) * (t * t)) +
            p2 * (t * t * t);
}
