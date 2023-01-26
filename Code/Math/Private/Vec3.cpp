#include "Math/Public/Vec3.h"
#include <cmath>
#include <iostream>

Vec3 operator+(const Vec3& l, const Vec3& r)
{
    return Vec3(l.x + r.x, l.y + r.y, l.z + r.z);
}

Vec3 operator-(const Vec3& l, const Vec3& r)
{
    return Vec3(l.x - r.x, l.y - r.y, l.z - r.z);
}

Vec3 operator*(const Vec3& v, float f)
{
    return Vec3(v.x * f, v.y * f, v.z * f);
}

Vec3 operator*(const Vec3& l, const Vec3& r)
{
    return Vec3(l.x * r.x, l.y * r.y, l.z * r.z);
}

bool operator==(const Vec3& l, const Vec3& r)
{
    Vec3 diff(l - r);
    return diff.LenSq() < VEC3_EPSILON;
}

bool operator!=(const Vec3& l, const Vec3& r)
{
    return !(l == r);
}

float Vec3::Dot(const Vec3& l, const Vec3& r)
{
    return l.x * r.x + l.y * r.y + l.z * r.z;
}

float Vec3::Angle(const Vec3& l, const Vec3& r)
{
    float sqMagL = l.x * l.x + l.y * l.y + l.z * l.z;
    float sqMagR = r.x * r.x + r.y * r.y + r.z * r.z;

    if (sqMagL < VEC3_EPSILON || sqMagR < VEC3_EPSILON)
    {
        return 0.0f;
    }

    float dot = l.x * r.x + l.y * r.y + l.z * r.z;
    float len = sqrtf(sqMagL) * sqrtf(sqMagR);
    return acosf(dot / len);
}

Vec3 Vec3::Project(const Vec3& a, const Vec3& b)
{
    float magBSq = b.Len();
    if (magBSq < VEC3_EPSILON)
    {
        return Vec3();
    }
    float scale = Dot(a, b) / magBSq;
    return b * scale;
}

Vec3 Vec3::Reject(const Vec3& a, const Vec3& b)
{
    Vec3 projection = Project(a, b);
    return a - projection;
}

Vec3 Vec3::Reflect(const Vec3& a, const Vec3& b)
{
    float magBSq = b.Len();
    if (magBSq < VEC3_EPSILON)
    {
        return Vec3();
    }
    float scale = Dot(a, b) / magBSq;
    Vec3 proj2 = b * (scale * 2);
    return a - proj2;
}

Vec3 Vec3::Cross(const Vec3& l, const Vec3& r)
{
    return Vec3(
        l.y * r.z - l.z * r.y,
        l.z * r.x - l.x * r.z,
        l.x * r.y - l.y * r.x
    );
}

Vec3 Vec3::Lerp(const Vec3& s, const Vec3& e, float t)
{
    return Vec3(
        s.x + (e.x - s.x) * t,
        s.y + (e.y - s.y) * t,
        s.z + (e.z - s.z) * t
    );
}

Vec3 Vec3::Slerp(const Vec3& s, const Vec3& e, float t)
{
    if (t < 0.01f)
    {
        return Lerp(s, e, t);
    }

    Vec3 from = s.Normalized();
    Vec3 to = e.Normalized();

    float theta = Angle(from, to);
    float sin_theta = sinf(theta);

    float a = sinf((1.0f - t) * theta) / sin_theta;
    float b = sinf(t * theta) / sin_theta;

    return from * a + to * b;
}

Vec3 Vec3::Nlerp(const Vec3& s, const Vec3& e, float t)
{
    Vec3 linear(
        s.x + (e.x - s.x) * t,
        s.y + (e.y - s.y) * t,
        s.z + (e.z - s.z) * t
    );
    return linear.Normalized();
}

float Vec3::LenSq() const
{
    return x * x + y * y + z * z;
}

float Vec3::Len() const
{
    float lenSq = x * x + y * y + z * z;
    if (lenSq < VEC3_EPSILON)
    {
        return 0.0f;
    }
    return sqrtf(lenSq);
}

void Vec3::Normalize()
{
    float lenSq = x * x + y * y + z * z;
    if (lenSq < VEC3_EPSILON)
    {
        return;
    }
    float invLen = 1.0f / sqrtf(lenSq);

    x *= invLen;
    y *= invLen;
    z *= invLen;
}

Vec3 Vec3::Normalized() const
{
    float lenSq = x * x + y * y + z * z;

    if (lenSq < VEC3_EPSILON)
    {
        return Vec3(x, y, z);
    }
    float invLen = 1.0f / sqrtf(lenSq);

    return Vec3(
        x * invLen,
        y * invLen,
        z * invLen
    );
}
