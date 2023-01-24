#include "Quat.h"
#include "Mat4.h"
#include <cmath>

#define QUAT_EPSILON 0.000001f

Quat operator+(const Quat& a, const Quat& b)
{
    return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};
}

Quat operator-(const Quat& a, const Quat& b)
{
    return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};
}

Quat operator*(const Quat& a, float b)
{
    return {a.x * b, a.y * b, a.z * b, a.w * b};
}

Quat operator-(const Quat& q)
{
    return {-q.x, -q.y, -q.z, -q.w};
}

Quat operator*(const Quat& Q1, const Quat& Q2) {
    return Quat(
        Q2.x * Q1.w + Q2.y * Q1.z - Q2.z * Q1.y + Q2.w * Q1.x,
        -Q2.x * Q1.z + Q2.y * Q1.w + Q2.z * Q1.x + Q2.w * Q1.y,
        Q2.x * Q1.y - Q2.y * Q1.x + Q2.z * Q1.w + Q2.w * Q1.z,
        -Q2.x * Q1.x - Q2.y * Q1.y - Q2.z * Q1.z + Q2.w * Q1.w
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
    const float angle = 2.0f * acosf(q.scalar);
    const Vec3 axis = q.vector.Normalized();

    const float halfCos = cos(f * angle * 0.5f);
    const float halfSin = sin(f * angle * 0.5f);

    return {axis.x * halfSin, axis.y * halfSin, axis.z * halfSin, halfCos};
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
    const Vec3 norm = axis.Normalized();

    const float halfAngle = angle * 0.5f;
    const float s = sinf(halfAngle);

    return {norm.x * s, norm.y * s, norm.z * s, cosf(halfAngle)};
}

Quat Quat::FromTo(const Vec3& from, const Vec3& to)
{
    const Vec3 fromN = from.Normalized();
    const Vec3 toN = to.Normalized();

    if(fromN == toN)
    {
        return {};
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

    const Vec3 half = (fromN + toN).Normalized();
    Vec3 axis = Vec3::Cross(fromN, half);
    return {axis.x, axis.y, axis.z, Vec3::Dot(fromN, half)};
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
    const float lenSq = q.LenSq();
    if(lenSq < QUAT_EPSILON)
    {
        return;
    }

    const float invLen = 1/sqrt(lenSq);
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
    const float lenSq = LenSq();
    if(lenSq <= QUAT_EPSILON)
    {
        return 0.0f;
    }

    return sqrt(lenSq);
}

Quat Quat::Conjugate()
{
    return {-x, -y, -z, w};
}

Quat Quat::Inverse() const
{
    const float lenSq = LenSq();
    if(lenSq < QUAT_EPSILON)
    {
        return {};
    }

    const float recip = 1.0f / lenSq;
    return {-x * recip, -y * recip, -z * recip, w * recip};
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
    const Vec3 forward = direction.Normalized();
    Vec3 up = refUp.Normalized();
    const Vec3 right = Vec3::Cross(up, forward);
    up = Vec3::Cross(forward, right);

    const Quat worldToObject = FromTo(Vec3(0,0,1), forward);
    const Vec3 objectUp = worldToObject * Vec3(0,1,0);
    const Quat objectUpToDesiredUp = FromTo(objectUp, up);

    const Quat result = worldToObject * objectUpToDesiredUp;
    return result.Normalized();
}

Mat4 Quat::ToMat4() const
{
    const Quat thisQuat = Quat(x,y,z,w);
    const Vec3 right = thisQuat * Vec3(1,0,0);
    const Vec3 up = thisQuat * Vec3(0,1,0);
    const Vec3 forward = thisQuat * Vec3(0,0,1);

    return {
        right.x, right.y, right.z, 0,
        up.x, up.y, up.z, 0,
        forward.x, forward.y, forward.z, 0,
        0,0,0,1
    };
}
