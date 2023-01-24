#pragma once

template<typename T>
class Bezier
{
public:
    T P1; // Point 1
    T C1; // Control 1
    T P2; // Point 2
    T C2; // Control 2

    T Evaluate(float t);
    static T Evaluate(T p1, T c1, T p2, T c2, float t);
};