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