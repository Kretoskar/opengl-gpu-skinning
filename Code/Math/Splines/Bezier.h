#pragma once
#include "Math/Vec3.h"

template<typename T>

class Bezier
{
public:
    T P1; // Point 1
    T C1; // Control 1
    T P2; // Point 2
    T C2; // Control 2

    T Interpolate(float t);
};
