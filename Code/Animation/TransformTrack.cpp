#include "TransformTrack.h"

TransformTrack::TransformTrack() : id(0) {}

TransformTrack::TransformTrack(unsigned id) : id(id) {}

unsigned int TransformTrack::GetId()
{
    return id;
}

void TransformTrack::SetId(unsigned int id)
{
    this->id = id;
}

VectorTrack& TransformTrack::GetPositionTrack()
{
    return position;
}

QuaternionTrack& TransformTrack::GetRotationTrack()
{
    return rotation;
}

VectorTrack& TransformTrack::GetScaleTrack()
{
    return scale;
}

bool TransformTrack::IsValid()
{
    return position.Size() > 1 || 
           rotation.Size() > 1 || 
           scale.Size() > 1;
}

float TransformTrack::GetStartTime()
{
    float result = 0.0f;
    bool isSet = false;

    if (position.Size() > 1)
    {
        result = position.GetStartTime();
        isSet = true;
    }

    if (rotation.Size() > 1)
    {
        const float rotationStart = rotation.GetStartTime();
        if (rotationStart < result || !isSet)
        {
            result = rotationStart;
            isSet = true;
        }
    }
    
    if (scale.Size() > 1)
    {
        float scaleStart = scale.GetStartTime();
        if (scaleStart < result || !isSet)
        {
            result = scaleStart;
        }
    }
    
    return result;
}

float TransformTrack::GetEndTime()
{
    float result = 0.0f;
    bool isSet = false;

    if (position.Size() > 1)
    {
        result = position.GetEndTime();
        isSet = true;
    }

    if (rotation.Size() > 1)
    {
        const float rotationEnd = rotation.GetEndTime();
        if (rotationEnd > result || !isSet)
        {
            result = rotationEnd;
            isSet = true;
        }
    }

    if (scale.Size() > 1)
    {
        const float scaleEnd = scale.GetEndTime();
        if (scaleEnd > result || !isSet) {
            result = scaleEnd;
        }
    }
    
    return result;
}

Transform TransformTrack::Sample(const Transform& ref, float time, bool loop)
{
    Transform result = ref; // Assign default values

    if (position.Size() > 1)
    {
        result.position = position.Sample(time, loop);
    }
    
    if (rotation.Size() > 1)
    { 
        result.rotation = rotation.Sample(time, loop);
    }
    
    if (scale.Size() > 1)
    {
        result.scale = scale.Sample(time, loop);
    }
    
    return result;
}