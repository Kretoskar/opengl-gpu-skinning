#pragma once

template<typename T>
class CubicHermite
{
public:
    T P1; // Point 1
    T S1; // Slope 1
    T P2; // Point 2
    T S2; // Slope 2

    T Evaluate(float t);
};
