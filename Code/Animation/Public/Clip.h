#pragma once

#include <vector>
#include <string>
#include "TransformTrack.h"
#include "Pose.h"

class Clip
{
protected:
    std::vector<TransformTrack> mTracks;
    std::string mName;
    float mStartTime;
    float mEndTime;
    bool mLooping;
    
    float AdjustTimeToFitRange(float inTime) const;
    
public:
    Clip();
    unsigned int GetIdAtIndex(unsigned int index);
    void SetIdAtIndex(unsigned int index, unsigned int id);
    unsigned int Size() const;
    float Sample(Pose& outPose, float inTime);
    TransformTrack& operator[](unsigned int index);
    void RecalculateDuration();
    std::string& GetName();
    void SetName(const std::string& inNewName);
    float GetDuration() const;
    float GetStartTime() const;
    float GetEndTime() const;
    bool GetLooping() const;
    void SetLooping(bool inLooping);
};
