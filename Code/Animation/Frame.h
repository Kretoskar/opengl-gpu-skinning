#pragma once

template<unsigned int N>
struct Frame {
    float mValue[N];
    float mIn[N];
    float mOut[N];
    float mTime;
};

typedef Frame<1> ScalarFrame;
typedef Frame<3> VectorFrame;
typedef Frame<4> QuaternionFrame;