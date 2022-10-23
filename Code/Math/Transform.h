#pragma once

#include "Quat.h"
#include "Vec3.h"

struct Transform
{
    Vec3 position;
    Quat rotation;
    Vec3 scale;

    Transform(const Vec3& position, const Quat& rotation, const Vec3& scale) :
        position(position), rotation(rotation), scale(scale) {}

    Transform() : position({0,0,0}), rotation({0,0,0,1}), scale({1, 1, 1}) {}

    static Transform Combine(const Transform& a, const Transform& b);
    static Transform Mix(const Transform& a, const Transform& b, float t);
    static Vec3 TransformPoint(const Transform& a, const Vec3& b);
    static Vec3 TransformVector(const Transform& a, const Vec3& b);
    
    Transform Inverse() const;
    Mat4 ToMat4();
};
