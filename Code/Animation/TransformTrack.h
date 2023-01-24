#pragma once
#include "Track.h"
#include "Math/Transform.h"

class TransformTrack
{
protected:
    unsigned int id;
    VectorTrack position;
    QuaternionTrack rotation;
    VectorTrack scale;
    
public:
    TransformTrack();
    TransformTrack(unsigned int id);
    
    unsigned int GetId();
    void SetId(unsigned int id);

    VectorTrack& GetPositionTrack();
    QuaternionTrack& GetRotationTrack();
    VectorTrack& GetScaleTrack();

    float GetStartTime();
    float GetEndTime();
    bool IsValid();

    Transform Sample(const Transform& ref, float time, bool looping);
}; 
