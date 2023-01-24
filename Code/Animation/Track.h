#pragma once
#include <vector>
#include "Frame.h"
#include "Interpolation.h"
#include "Math/Quat.h"
#include "Math/Vec3.h"

template<typename T, int N>
class Track
{
protected:
    std::vector<Frame<N>> mFrames;
    Interpolation mInterpolation;

public:
    Track();

    void Resize(unsigned int size);
    unsigned int Size();
    Interpolation GetInterpolation() const;
    void SetInterpolation(Interpolation interp);

    float GetStartTime();
    float GetEndTime();

    T Sample(float time, bool looping);
    Frame<N>& operator[](unsigned int index);

    T Hermite(float t, const T& p1, const T& s1, const T& _p2, const T& s2);
    
    int FrameIndex(float time, bool looping);
    float AdjustTimeToFitTrack(float t, bool loop);

    T Cast(float* value);

    T SampleConstant(float t, bool loop);
    T SampleLinear(float time, bool looping);
    T SampleCubic(float time, bool looping);
};

typedef Track<float, 1> ScalarTrack;
typedef Track<Vec3, 3> VectorTrack;
typedef Track<Quat, 4> QuaternionTrack;