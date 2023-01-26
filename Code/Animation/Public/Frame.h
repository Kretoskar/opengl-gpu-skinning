#pragma once

template <unsigned int N>
class Frame
{
public:
    float mValue[N];
    float mIn[N];
    float mOut[N];
    float mTime;
};

using ScalarFrame = Frame<1>;
using VectorFrame = Frame<3>;
using QuaternionFrame = Frame<4>;