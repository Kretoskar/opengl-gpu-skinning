#pragma once

#include <string>
#include <vector>

class Pose;
class TransformTrack;

class Clip
{
protected:
    std::vector<TransformTrack> tracks;
    std::string name;
    float startTime;
    float endTime;
    bool looping;

    float AdjustTimeToFitRange(float inTime);

public:
    Clip();
    unsigned int GetIdAtIndex(unsigned int index);
    void SetIdAtIndex(unsigned int idx, unsigned int id);
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
