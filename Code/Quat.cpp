#include "Quat.h"
#include "Mat4.h"
#include <cmath>

#define QUAT_EPSILON 0.000001f

Quat Quat::AngleAxis(float angle, const Vec3& axis)
{
    Vec3 norm = Vec3::Normalized(axis);

    float halfAngle = angle * 0.5f;
    float s = sinf(halfAngle);

    return Quat(norm.x * s, norm.y * s, norm.z * s, cosf(halfAngle));
}

Quat Quat::FromTo(const Vec3& from, const Vec3& to)
{
    Vec3 fromN = Vec3::Normalized(from);
    Vec3 toN = Vec3::Normalized(to);

    if(fromN == toN)
    {
        return Quat();
    }
    
    // if vectors are orthogonal, return the "flattest" axis
    if (fromN == (toN * -1.0f))
    {
        Vec3 ortho = Vec3(1,0,0);
        if(fabsf(fromN.y) < fabsf(fromN.x))
        {
            ortho = Vec3(0,1,0);
        }
        if(fabsf(fromN.z) < fabsf(fromN.y) && fabsf(fromN.z) < fabsf(fromN.x))
        {
            ortho = Vec3(0,0,1);
        }
        Vec3 axis = Vec3::Normalized(Vec3::Cross(fromN, ortho));
        return Quat(axis.x, axis.y, axis.z, 0);
    }
    
    Vec3 half = Vec3::Normalized(fromN + toN);
    Vec3 axis = Vec3::Cross(fromN, half);
    return Quat(axis.x, axis.y, axis.z, Vec3::Dot(fromN, half));
}

Vec3 Quat::GetAxis(const Quat& quat)
{
    return Vec3::Normalized(Vec3(quat.x, quat.y, quat.z));
}

float Quat::GetAngle(const Quat& quat)
{
    return 2.0f * acosf(quat.w);
}

bool Quat::SameOrientation(const Quat& a, const Quat& b)
{
    return (fabsf(a.x - b.x) <= QUAT_EPSILON &&
            fabsf(a.y - b.y) <= QUAT_EPSILON &&
            fabsf(a.z - b.z) <= QUAT_EPSILON &&
            fabsf(a.w - b.w) <= QUAT_EPSILON) ||
            (fabsf(a.x + b.x) <= QUAT_EPSILON &&
            fabsf(a.y + b.y) <= QUAT_EPSILON &&
            fabsf(a.z + b.z) <= QUAT_EPSILON &&
            fabsf(a.w + b.w) <= QUAT_EPSILON);
}

Quat operator+(const Quat& a, const Quat& b)
{
    return Quat(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w);
}

Quat operator-(const Quat& a, const Quat& b)
{
    return Quat(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w);
}

Quat operator*(const Quat& a, float b)
{
    return Quat(a.x * b, a.y * b, a.z * b, a.w * b);
}

Quat operator-(const Quat& q)
{
    return Quat(-q.x, -q.y, -q.z, -q.w);
}

bool operator==(const Quat& left, const Quat& right)
{
    return (fabsf(left.x - right.x) <= QUAT_EPSILON &&
            fabsf(left.y - right.y) <= QUAT_EPSILON &&
            fabsf(left.z - right.z) <= QUAT_EPSILON &&
            fabsf(left.w - right.w) <= QUAT_EPSILON);
}

bool operator!=(const Quat& left, const Quat& right)
{
    return !(left == right);
}
