#include "Bezier.h"

template <typename T>
T Bezier<T>::Evaluate(float t)
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