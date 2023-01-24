#include "Track.h"

template Track<float, 1>;
template Track<Vec3, 3>;
template Track<Quat, 4>;

namespace TrackHelpers
{
    float Interpolate(float a, float b, float t)
    {
        return a + (b - a) * t;
    }
    
    Vec3 Interpolate(const Vec3& a, const Vec3& b, float t)
    {
        return Vec3::Lerp(a, b, t);
    }
    
    Quat Interpolate(const Quat& a, const Quat& b, float t)
    {
        Quat result = Quat::Mix(a, b, t);
        if (Quat::Dot(a,b) < 0)
        { // Neighborhood
            result = Quat::Mix(a, -b, t);
        }
        
        return result.Normalized(); //NLerp
    }
    
    float AdjustHermiteResult(float f) {
        return f;
    }

    Vec3 AdjustHermiteResult(const Vec3& v) {
        return v;
    }
    
    Quat AdjustHermiteResult(const Quat& q) {
        return q.Normalized();
    }
    
    void Neighborhood(const float& a, float& b){}

    void Neighborhood(const Vec3& a, Vec3& b){}

    void Neighborhood(const Quat& a, Quat& b)
    {
        if (Quat::Dot(a, b) < 0)
        {
            b = -b;
        }
    }
};

template<typename T, int N>
Track<T, N>::Track()
{
    mInterpolation = Interpolation::Linear;
}

template<typename T, int N>
float Track<T, N>::GetStartTime()
{
    return mFrames[0].mTime;
}

template<typename T, int N>
float Track<T, N>::GetEndTime()
{
    return mFrames[mFrames.size() - 1].mTime;
}

template<typename T, int N>
T Track<T, N>::Sample(float time, bool looping)
{
    if (mInterpolation == Interpolation::Constant)
    {
        return SampleConstant(time, looping);
    }

    if (mInterpolation == Interpolation::Linear)
    {
        return SampleLinear(time, looping);

    }
    return SampleCubic(time, looping);
}

template<typename T, int N>
Frame<N>& Track<T, N>::operator[](unsigned int index)
{
    return mFrames[index];
}

template <typename T, int N>
T Track<T, N>::Hermite(float t, const T& p1, const T& s1, const T& _p2, const T& s2)
{
    float tt = t * t;
    float ttt = tt * t;

    T p2 = _p2;

    TrackHelpers::Neighborhood(p1, p2);

    float h1 = 2.0f * ttt - 3.0f * tt + 1.0f;
    float h2 = -2.0f * ttt + 3.0f * tt;
    float h3 = ttt - 2.0f * tt + t;
    float h4 = ttt - tt;

    T result = p1 * h1 + p2 * h2 + s1 * h3 + s2 * h4;
    
    //Adjust Hermite result will normalize if input is quaternion
    return TrackHelpers::AdjustHermiteResult(result);
}

template<typename T, int N>
void Track<T, N>::Resize(unsigned int size)
{
    mFrames.resize(size);
}

template<typename T, int N>
unsigned int Track<T, N>::Size()
{
    return mFrames.size();
}

template<typename T, int N>
Interpolation Track<T, N>::GetInterpolation() const
{
    return mInterpolation;
}

template<typename T, int N>
void Track<T, N>::SetInterpolation(Interpolation interpolation)
{
    mInterpolation = interpolation;
}

template<typename T, int N>
int Track<T, N>::FrameIndex(float time, bool looping)
{
    unsigned int size = static_cast<unsigned int>(mFrames.size());

    if (size <= 1)
    {
        return -1;
    }

    if (looping)
    {
        const float startTime = mFrames[0].mTime;
        const float endTime = mFrames[size - 1].mTime;
        float duration = endTime - startTime;

        time = fmodf(time - startTime, 
                     endTime - startTime);

        if (time < 0.0f)
        {
            time += endTime - startTime;
        }

        time = time + startTime;
    }
    else
    {
        if (time <= mFrames[0].mTime)
        {
            return 0;
        }

        if (time >= mFrames[size - 2].mTime)
        {
            return static_cast<int>(size) - 2;
        }
    }

    for (int i = static_cast<int>(size) - 1; i >= 0; --i)
    {
        if (time >= mFrames[i].mTime)
        {
            return i;
        }
    }
    
    return -1;
}

// This function is intended to be called when the playback time of an animation changes.
template<typename T, int N>
float Track<T, N>::AdjustTimeToFitTrack(float time, bool looping)
{
    unsigned int size = static_cast<unsigned int>(mFrames.size());

    if (size <= 1)
    { 
        return 0.0f; 
    }

    const float startTime = mFrames[0].mTime;
    const float endTime = mFrames[size - 1].mTime;
    const float duration = endTime - startTime;

    if (duration <= 0.0f)
    { 
        return 0.0f; 
    }

    if (looping)
    {
        time = fmodf(time - startTime, 
                     endTime - startTime);

        if (time < 0.0f)
        {
            time += endTime - startTime;
        }

        time = time + startTime;
    }

    else
    {
        if (time <= mFrames[0].mTime)
        { 
            time = startTime;  
        }
        
        if (time >= mFrames[size - 1].mTime)
        { 
            time = endTime; 
        }
    }

    return time;
}

template<>
float Track<float, 1>::Cast(float* value)
{
    return value[0];
}

template<>
Vec3 Track<Vec3, 3>::Cast(float* value)
{
    return {value[0], value[1], value[2]};
}

template<>
Quat Track<Quat, 4>::Cast(float* value)
{
    const Quat r = Quat(value[0], value[1], value[2], value[3]);
    return r.Normalized();
}

template <typename T, int N>
T Track<T, N>::SampleConstant(float t, bool loop)
{
    int frame = FrameIndex(t, loop);
    
    if (frame < 0 || frame >= static_cast<int>(mFrames.size()))
    {
        return T();
    }
    
    return Cast(&mFrames[frame].mValue[0]);
}

template <typename T, int N>
T Track<T, N>::SampleLinear(float time, bool looping)
{
    const int thisFrame = FrameIndex(time, looping);

    if (thisFrame < 0 || thisFrame >= mFrames.size() - 1)
    {
        return T();
    }

    const int nextFrame = thisFrame + 1;
    const float trackTime = AdjustTimeToFitTrack(time, looping);
    const float thisTime = mFrames[thisFrame].mTime;
    const float frameDelta = mFrames[nextFrame].mTime - thisTime;

    if (frameDelta <= 0.0f)
    {
        return T();
    }

    const float t = (trackTime - thisTime) / frameDelta;
    const T start = Cast(&mFrames[thisFrame].mValue[0]);
    const T end = Cast(&mFrames[nextFrame].mValue[0]);

    return TrackHelpers::Interpolate(start, end, t);
}

template <typename T, int N>
T Track<T, N>::SampleCubic(float time, bool looping)
{
    const int thisFrame = FrameIndex(time, looping);
    if (thisFrame < 0 || thisFrame >= mFrames.size() - 1)
    {
        return T();
    }

    const int nextFrame = thisFrame + 1;
    const float trackTime = AdjustTimeToFitTrack(time, looping);
    const float thisTime = mFrames[thisFrame].mTime;
    const float frameDelta = mFrames[nextFrame].mTime - thisTime;

    if (frameDelta <= 0.0f)
    {
        return T();
    }

    const float t = (trackTime - thisTime) / frameDelta;
    const size_t fltSize = sizeof(float);

    const T point1 = Cast(&mFrames[thisFrame].mValue[0]);
    T slope1;// = mFrames[thisFrame].mOut * frameDelta;
    memcpy(&slope1, mFrames[thisFrame].mOut, N * fltSize);
    slope1 = slope1 * frameDelta;
    
    const T point2 = Cast(&mFrames[nextFrame].mValue[0]);
    T slope2;// = mFrames[nextFrame].mIn[0] * frameDelta;
    memcpy(&slope2, mFrames[nextFrame].mIn, N * fltSize);
    slope2 = slope2 * frameDelta;

    return Hermite(t, point1, slope1, point2, slope2);
}