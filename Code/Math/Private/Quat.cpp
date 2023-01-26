#include "Math/Public/Quat.h"
#include <cmath>
#include <iostream>

Quat Quat::AngleAxis(float angle, const Vec3& axis)
{
    Vec3 norm = axis.Normalized();
    float s = sinf(angle * 0.5f);

    return Quat(
        norm.x * s,
        norm.y * s,
        norm.z * s,
        cosf(angle * 0.5f)
    );
}

Quat Quat::FromTo(const Vec3& from, const Vec3& to)
{
    Vec3 f = from.Normalized();
    Vec3 t = to.Normalized();

    if (f == t)
    {
        return Quat();
    }
    if (f == t * -1.0f)
    {
        auto ortho = Vec3(1, 0, 0);
        if (fabsf(f.y) < fabsf(f.x))
        {
            ortho = Vec3(0, 1, 0);
        }
        if (fabsf(f.z) < fabs(f.y) && fabs(f.z) < fabsf(f.x))
        {
            ortho = Vec3(0, 0, 1);
        }

        Vec3 axis = Vec3::Cross(f, ortho).Normalized();
        return Quat(axis.x, axis.y, axis.z, 0);
    }

    Vec3 half = (f + t).Normalized();
    Vec3 axis = Vec3::Cross(f, half);

    return Quat(
        axis.x,
        axis.y,
        axis.z,
        Vec3::Dot(f, half)
    );
}

Vec3 Quat::GetAxis() const
{
    return Vec3(x, y, z).Normalized();
}

float Quat::GetAngle() const
{
    return 2.0f * acosf(w);
}

Quat operator+(const Quat& a, const Quat& b)
{
    return Quat(
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
        a.w + b.w
    );
}

Quat operator-(const Quat& a, const Quat& b)
{
    return Quat(
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
        a.w - b.w
    );
}

Quat operator*(const Quat& a, float b)
{
    return Quat(
        a.x * b,
        a.y * b,
        a.z * b,
        a.w * b
    );
}

Quat operator-(const Quat& q)
{
    return Quat(
        -q.x,
        -q.y,
        -q.z,
        -q.w
    );
}

bool operator==(const Quat& left, const Quat& right)
{
    return (fabsf(left.x - right.x) <= QUAT_EPSILON && fabsf(left.y - right.y) <= QUAT_EPSILON &&
        fabsf(left.z - right.z) <= QUAT_EPSILON && fabsf(left.w - left.w) <= QUAT_EPSILON);
}

bool operator!=(const Quat& a, const Quat& b)
{
    return !(a == b);
}

bool Quat::SameOrientation(const Quat& left, const Quat& right)
{
    return (fabsf(left.x - right.x) <= QUAT_EPSILON && fabsf(left.y - right.y) <= QUAT_EPSILON &&
            fabsf(left.z - right.z) <= QUAT_EPSILON && fabsf(left.w - left.w) <= QUAT_EPSILON)
        || (fabsf(left.x + right.x) <= QUAT_EPSILON && fabsf(left.y + right.y) <= QUAT_EPSILON &&
            fabsf(left.z + right.z) <= QUAT_EPSILON && fabsf(left.w + left.w) <= QUAT_EPSILON);
}

float Quat::Dot(const Quat& a, const Quat& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float Quat::LenSq() const
{
    return x * x + y * y + z * z + w * w;
}

float Quat::Len() const
{
    float lenSq = x * x + y * y + z * z + w * w;
    if (lenSq < QUAT_EPSILON)
    {
        return 0.0f;
    }
    return sqrtf(lenSq);
}

void Quat::Normalize()
{
    float lenSq = x * x + y * y + z * z + w * w;
    if (lenSq < QUAT_EPSILON)
    {
        return;
    }
    float i_len = 1.0f / sqrtf(lenSq);

    x *= i_len;
    y *= i_len;
    z *= i_len;
    w *= i_len;
}

Quat Quat::Normalized() const
{
    float lenSq = x * x + y * y + z * z + w * w;
    if (lenSq < QUAT_EPSILON)
    {
        return Quat();
    }
    float i_len = 1.0f / sqrtf(lenSq);

    return Quat(
        x * i_len,
        y * i_len,
        z * i_len,
        w * i_len
    );
}

Quat Quat::Conjugate() const
{
    return Quat(
        -x,
        -y,
        -z,
        w
    );
}

Quat Quat::Inverse() const
{
    float lenSq = x * x + y * y + z * z + w * w;
    if (lenSq < QUAT_EPSILON)
    {
        return Quat();
    }
    float recip = 1.0f / lenSq;

    // conjugate / norm
    return Quat(
        -x * recip,
        -y * recip,
        -z * recip,
        w * recip
    );
}

Quat operator*(const Quat& Q1, const Quat& Q2)
{
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

Quat Quat::Mix(const Quat& from, const Quat& to, float t)
{
    return from * (1.0f - t) + to * t;
}

Quat Quat::Nlerp(const Quat& from, const Quat& to, float t)
{
    return (from + (to - from) * t).Normalized();
}

Quat operator^(const Quat& q, float f)
{
    float angle = 2.0f * acosf(q.scalar);
    Vec3 axis = (q.vector).Normalized();

    float halfCos = cosf(f * angle * 0.5f);
    float halfSin = sinf(f * angle * 0.5f);

    return Quat(
        axis.x * halfSin,
        axis.y * halfSin,
        axis.z * halfSin,
        halfCos
    );
}

Quat Quat::Slerp(const Quat& start, const Quat& end, float t)
{
    if (fabsf(Dot(start, end)) > 1.0f - QUAT_EPSILON)
    {
        return Nlerp(start, end, t);
    }

    return (((start.Inverse() * end) ^ t) * start).Normalized();
}

Quat Quat::LookRotation(const Vec3& direcion, const Vec3& up)
{
    // Find orthonormal basis vectors
    Vec3 f = (direcion).Normalized();
    Vec3 u = (up).Normalized();
    Vec3 r = Vec3::Cross(u, f);
    u = Vec3::Cross(f, r);

    // From world forward to object forward
    Quat f2d = FromTo(Vec3(0, 0, 1), f);

    // what direction is the new object up?
    Vec3 objectUp = f2d * Vec3(0, 1, 0);
    // From object up to desired up
    Quat u2u = FromTo(objectUp, u);

    // Rotate to forward direction first, then twist to correct up
    Quat result = f2d * u2u;
    // Don’t forget to normalize the result
    return result.Normalized();
}

Mat4 Quat::ToMat4()
{
    Vec3 r = *this * Vec3(1, 0, 0);
    Vec3 u = *this * Vec3(0, 1, 0);
    Vec3 f = *this * Vec3(0, 0, 1);

    return Mat4(
        r.x, r.y, r.z, 0,
        u.x, u.y, u.z, 0,
        f.x, f.y, f.z, 0,
        0, 0, 0, 1
    );
}
