#pragma once

#include "Math/Public/Vec3.h"
#include "Math/Public/Mat4.h"
#include "Math/Public/Quat.h"

struct Transform
{
    Vec3 position;
    Quat rotation;
    Vec3 scale;

    Transform() :
        position(Vec3(0, 0, 0)),
        rotation(Quat(0, 0, 0, 1)),
        scale(Vec3(1, 1, 1))
    {
    }

    Transform(const Vec3& p, const Quat& r, const Vec3& s) :
        position(p), rotation(r), scale(s)
    {
    }

    static Transform Combine(const Transform& a, const Transform& b);
    static Transform Mix(const Transform& a, const Transform& b, float t);

    Vec3 TransformPoint(const Vec3& b);
    Vec3 TransformVector(const Vec3& b);
    Transform Inverse();
    Mat4 ToMat4();
};


bool operator==(const Transform& a, const Transform& b);
bool operator!=(const Transform& a, const Transform& b);
