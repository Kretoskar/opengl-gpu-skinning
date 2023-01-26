#include "Animation/Public/Clip.h"

Clip::Clip()
{
    mName = "None";
    mStartTime = 0.0f;
    mEndTime = 0.0f;
    mLooping = true;
}

float Clip::Sample(Pose& outPose, float time)
{
    if (GetDuration() == 0.0f)
    {
        return 0.0f;
    }
    time = AdjustTimeToFitRange(time);

    unsigned int size = mTracks.size();
    for (unsigned int i = 0; i < size; ++i)
    {
        unsigned int joint = mTracks[i].GetId();
        Transform local = outPose.GetLocalTransform(joint);
        Transform animated = mTracks[i].Sample(local, time, mLooping);
        outPose.SetLocalTransform(joint, animated);
    }
    return time;
}

float Clip::AdjustTimeToFitRange(float inTime) const
{
    if (mLooping)
    {
        const float duration = mEndTime - mStartTime;
        if (duration <= 0)
        {
            return 0.0f;
        }
        
        inTime = fmodf(inTime - mStartTime, mEndTime - mStartTime);
        if (inTime < 0.0f)
        {
            inTime += mEndTime - mStartTime;
        }
        inTime = inTime + mStartTime;
    }
    else
    {
        if (inTime < mStartTime)
        {
            inTime = mStartTime;
        }
        if (inTime > mEndTime)
        {
            inTime = mEndTime;
        }
    }
    return inTime;
}

void Clip::RecalculateDuration()
{
    mStartTime = 0.0f;
    mEndTime = 0.0f;
    bool startSet = false;
    bool endSet = false;
    const unsigned int tracksSize = static_cast<unsigned>(mTracks.size());
    for (unsigned int i = 0; i < tracksSize; ++i)
    {
        if (mTracks[i].IsValid())
        {
            const float trackStartTime = mTracks[i].GetStartTime();
            const float trackEndTime = mTracks[i].GetEndTime();

            if (trackStartTime < mStartTime || !startSet)
            {
                mStartTime = trackStartTime;
                startSet = true;
            }

            if (trackEndTime > mEndTime || !endSet)
            {
                mEndTime = trackEndTime;
                endSet = true;
            }
        }
    }
}

TransformTrack& Clip::operator[](unsigned int joint)
{
    for (auto& mTrack : mTracks)
    {
        if (mTrack.GetId() == joint)
        {
            return mTrack;
        }
    }

    mTracks.emplace_back();
    mTracks[mTracks.size() - 1].SetId(joint);
    return mTracks[mTracks.size() - 1];
}

std::string& Clip::GetName()
{
    return mName;
}

void Clip::SetName(const std::string& inNewName)
{
    mName = inNewName;
}

unsigned int Clip::GetIdAtIndex(unsigned int index)
{
    return mTracks[index].GetId();
}

void Clip::SetIdAtIndex(unsigned int index, unsigned int id)
{
    return mTracks[index].SetId(id);
}

unsigned int Clip::Size() const
{
    return static_cast<unsigned>(mTracks.size());
}

float Clip::GetDuration() const
{
    return mEndTime - mStartTime;
}

float Clip::GetStartTime() const
{
    return mStartTime;
}

float Clip::GetEndTime() const
{
    return mEndTime;
}

bool Clip::GetLooping() const
{
    return mLooping;
}

void Clip::SetLooping(bool inLooping)
{
    mLooping = inLooping;
}
