#pragma once

#include <vector>
#include "Frame.h"
#include "Math/Public/Vec3.h"
#include "Math/Public/Quat.h"
#include "Interpolation.h"

template <typename T, int N>
class Track
{
protected:
    std::vector<Frame<N>> mFrames;
    Interpolation mInterpolation;
protected:
    T SampleConstant(float time, bool looping);
    T SampleLinear(float time, bool looping);
    T SampleCubic(float time, bool looping);
    T Hermite(float time, const T& point1, const T& slope1, const T& point2, const T& slope2);
    int FrameIndex(float time, bool looping);
    float AdjustTimeToFitTrack(float time, bool looping);
    T Cast(float* value);
public:
    Track();
    void Resize(unsigned int size);
    unsigned int Size();
    Interpolation GetInterpolation() const;
    void SetInterpolation(Interpolation interpolation);
    float GetStartTime();
    float GetEndTime();
    T Sample(float time, bool looping);
    Frame<N>& operator[](unsigned int index);
};

using ScalarTrack = Track<float, 1>;
using VectorTrack = Track<Vec3, 3>;
using QuaternionTrack = Track<Quat, 4>;