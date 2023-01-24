#include "Clip.h"

#include "Pose.h"
#include "TransformTrack.h"
#include "Math/Transform.h"

Clip::Clip()
{
    name = "No name given";
    startTime = 0.0f;
    endTime = 0.0f;
    looping = true;
}

float Clip::Sample(Pose& outPose, float time)
{
    if (GetDuration() == 0.0f)
    {
        return 0.0f;
    }

    time = AdjustTimeToFitRange(time);

    unsigned int size = tracks.size();

    for (unsigned int i = 0; i < size; ++i)
    {
        unsigned int j = tracks[i].GetId(); // Joint
        Transform local = outPose.GetLocalTransform(j);
        Transform animated = tracks[i].Sample(
                             local, time, looping);
        outPose.SetLocalTransform(j, animated);
    }
    
    return time;
}

float Clip::AdjustTimeToFitRange(float inTime)
{
    if (looping)
    {
        const float duration = endTime - startTime;
        
        if (duration <= 0)
        {
            return  0.0f;
        }

        inTime = fmodf(inTime - startTime, endTime - startTime);
        
        if (inTime < 0.0f)
        {
            inTime += endTime - startTime;
        }

        inTime = inTime + startTime;
    }
    else
    {
        if (inTime < startTime)
        {
            inTime = startTime;
        }
        
        if (inTime > endTime)
        {
            inTime = endTime;
        }
    }
    
    return inTime;
}

void Clip::RecalculateDuration()
{
    startTime = 0.0f;
    endTime = 0.0f;

    bool startSet = false;
    bool endSet = false;

    const unsigned int tracksSize = tracks.size();

    for (unsigned int i = 0; i < tracksSize; ++i)
    {
        if (tracks[i].IsValid())
        {
            float startTime = tracks[i].GetStartTime();
            float endTime = tracks[i].GetEndTime();

            if (startTime < startTime || !startSet)
            {
                startTime = startTime;
                startSet = true;
            }

            if (endTime > endTime || !endSet)
            {
                endTime = endTime;
                endSet = true;
            }
        }
    }
}

TransformTrack& Clip::operator[](unsigned int joint)
{
    for (auto& mTrack : tracks)
    {
        if (mTrack.GetId() == joint)
        {
            return mTrack;
        }
    }
    
    tracks.emplace_back();
    tracks[tracks.size() - 1].SetId(joint);

    return tracks[tracks.size() - 1];
}

std::string& Clip::GetName()
{
    return name;
}

unsigned int Clip::GetIdAtIndex(unsigned int index)
{
    return tracks[index].GetId();
}

unsigned int Clip::Size() const
{
    return static_cast<unsigned int>(tracks.size());
}

float Clip::GetDuration() const
{
    return endTime - startTime;
}

float Clip::GetStartTime() const
{
    return startTime;
}

float Clip::GetEndTime() const
{
    return endTime;
}

bool Clip::GetLooping() const
{
    return looping;
}

void Clip::SetName(const std::string& inNewName)
{
    name = inNewName;
}

void Clip::SetIdAtIndex(unsigned int index, unsigned int id)
{
    return tracks[index].SetId(id);
}

void Clip::SetLooping(bool inLooping)
{
    looping = inLooping;
}