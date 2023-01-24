#include "CubicHermite.h"

template <typename T>
T CubicHermite<T>::Evaluate(float t)
{
    return 
      P1 * ((1.0f + 2.0f * t) * ((1.0f - t) * (1.0f - t))) +
      S1 * (t * ((1.0f - t) * (1.0f - t))) +
      P2 * ((t * t) * (3.0f - 2.0f * t)) +
      S2 * ((t * t) * (t - 1.0f));
}

template <typename T>
T CubicHermite<T>::Evaluate(T p1, T s1, T p2, T s2, float t)
{
    return 
         p1 * ((1.0f + 2.0f * t) * ((1.0f - t) * (1.0f - t))) +
         s1 * (t * ((1.0f - t) * (1.0f - t))) +
         p2 * ((t * t) * (3.0f - 2.0f * t)) +
         s2 * ((t * t) * (t - 1.0f));   
}
