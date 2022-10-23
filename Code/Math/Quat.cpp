#include "Quat.h"
#include "Mat4.h"
#include <cmath>

#define QUAT_EPSILON 0.000001f

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

Quat operator*(const Quat&a, const Quat&b)
{
    return Quat(
        b.x *a.w + b.y*a.z - b.z*a.y + b.w*a.x,
        -b.x*a.z + b.y*a.w + b.z*a.x + b.w*a.y,
        b.x*a.y - b.y*a.x + b.z*a.w + b.w*a.z,
        -b.x*a.x - b.y*a.y - b.z*a.z + b.w*a.w
    );
}

Vec3 operator*(const Quat& q, const Vec3& v)
{
    return q.vector * 2.0f * Vec3::Dot(q.vector, v) +
        v * (q.scalar * q.scalar - Vec3::Dot(q.vector, q.vector)) +
            Vec3::Cross(q.vector, v) * 2.0f * q.scalar;
}

Quat operator^(const Quat& q, float f)
{
    float angle = 2.0f * acosf(q.scalar);
    Vec3 axis = q.vector.Normalized();

    float halfCos = cos(f * angle * 0.5f);
    float halfSin = sin(f * angle * 0.5f);

    return Quat(axis.x * halfSin, axis.y * halfSin, axis.z * halfSin, halfCos);
}

bool operator==(const Quat& left, const Quat& right)
{
    return (abs(left.x - right.x) <= QUAT_EPSILON &&
            abs(left.y - right.y) <= QUAT_EPSILON &&
            abs(left.z - right.z) <= QUAT_EPSILON &&
            abs(left.w - right.w) <= QUAT_EPSILON);
}

bool operator!=(const Quat& left, const Quat& right)
{
    return !(left == right);
}


Quat Quat::AngleAxis(float angle, const Vec3& axis)
{
    Vec3 norm = axis.Normalized();

    float halfAngle = angle * 0.5f;
    float s = sinf(halfAngle);

    return Quat(norm.x * s, norm.y * s, norm.z * s, cosf(halfAngle));
}

Quat Quat::FromTo(const Vec3& from, const Vec3& to)
{
    Vec3 fromN = from.Normalized();
    Vec3 toN = to.Normalized();

    if(fromN == toN)
    {
        return Quat();
    }
    
    // if vectors are orthogonal, return the "flattest" axis
    if (fromN == (toN * -1.0f))
    {
        Vec3 ortho = Vec3(1,0,0);
        if(abs(fromN.y) < abs(fromN.x))
        {
            ortho = Vec3(0,1,0);
        }
        if(abs(fromN.z) < abs(fromN.y) && abs(fromN.z) < abs(fromN.x))
        {
            ortho = Vec3(0,0,1);
        }
        Vec3 axis = Vec3::Cross(fromN, ortho).Normalized();
        return Quat(axis.x, axis.y, axis.z, 0);
    }
    
    Vec3 half = (fromN + toN).Normalized();
    Vec3 axis = Vec3::Cross(fromN, half);
    return Quat(axis.x, axis.y, axis.z, Vec3::Dot(fromN, half));
}

Vec3 Quat::GetAxis(const Quat& quat)
{
    return Vec3(quat.x, quat.y, quat.z).Normalized();
}

float Quat::GetAngle(const Quat& quat)
{
    return 2.0f * acosf(quat.w);
}

Quat Quat::Normalized() const
{
    float lenSq = LenSq();
    if(lenSq < QUAT_EPSILON)
    {
        return {x,y,z,w};
    }

    float invLen = 1/sqrt(lenSq);

    return Quat(x * invLen, y * invLen, z * invLen, w * invLen);
}

void Quat::Normalize(Quat& q)
{
    float lenSq = q.LenSq();
    if(lenSq < QUAT_EPSILON)
    {
        return;
    }

    float invLen = 1/sqrt(lenSq);
    q.x *= invLen;
    q.y *= invLen;
    q.z *= invLen;
    q.w *= invLen;
}

bool Quat::SameOrientation(const Quat& a, const Quat& b)
{
    return (abs(a.x - b.x) <= QUAT_EPSILON &&
            abs(a.y - b.y) <= QUAT_EPSILON &&
            abs(a.z - b.z) <= QUAT_EPSILON &&
            abs(a.w - b.w) <= QUAT_EPSILON) ||
            (abs(a.x + b.x) <= QUAT_EPSILON &&
            abs(a.y + b.y) <= QUAT_EPSILON &&
            abs(a.z + b.z) <= QUAT_EPSILON &&
            abs(a.w + b.w) <= QUAT_EPSILON);
}

float Quat::Dot(const Quat& a, const Quat& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float Quat::LenSq() const
{
    return x*x + y*y + z*z + w*w;
}

float Quat::Len() const
{
    float lenSq = LenSq();
    if(lenSq <= QUAT_EPSILON)
    {
        return 0.0f;
    }

    return sqrt(lenSq);
}

Quat Quat::Conjugate()
{
    return Quat(-x, -y, -z, w);
}

Quat Quat::Inverse() const
{
    float lenSq = LenSq();
    if(lenSq < QUAT_EPSILON)
    {
        return Quat();
    }

    float recip = 1.0f / lenSq;
    return Quat(-x * recip, -y * recip, -z * recip, w * recip);
}

Quat Quat::Mix(const Quat& from, const Quat& to, float t)
{
    return from * (1.0f - t) + to * t;
}

Quat Quat::Nlerp(const Quat& from, const Quat& to, float t)
{
    return (from + (to - from) * t).Normalized();
}

Quat Quat::Slerp(const Quat& start, const Quat& end, float t)
{
    if(abs(Dot(start, end)) > 1.0f - QUAT_EPSILON)
    {
        return Nlerp(start, end, t);
    }

    Quat delta = start.Inverse() * end;
    return ((delta ^ t) * start);
}

Quat Quat::LookRotation(const Vec3& direction, const Vec3& refUp)
{
    Vec3 forward = direction.Normalized();
    Vec3 up = refUp.Normalized();
    Vec3 right = Vec3::Cross(up, forward);
    up = Vec3::Cross(forward, right);

    Quat worldToObject = FromTo(Vec3(0,0,1), forward);
    Vec3 objectUp = worldToObject * Vec3(0,1,0);
    Quat objectUpToDesiredUp = FromTo(objectUp, up);

    Quat result = worldToObject * objectUpToDesiredUp;
    return result.Normalized();
}

Mat4 Quat::ToMat4() const
{
    Quat thisQuat = Quat(x,y,z,w);
    Vec3 right = thisQuat * Vec3(1,0,0);
    Vec3 up = thisQuat * Vec3(0,1,0);
    Vec3 forward = thisQuat * Vec3(0,0,1);

    return Mat4(
        right.x, right.y, right.z, 0,
        up.x, up.y, up.z, 0,
        forward.x, forward.y, forward.z, 0,
        0,0,0,1
        );
}
