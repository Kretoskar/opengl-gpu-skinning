#include "Vec3.h"
#include <corecrt_math.h>

#include <cmath>

#define VEC3_EPSILON 0.000001f
#define RAD_TO_DEG 57.2958f

Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
    return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

Vec3 operator*(const Vec3& v, float f)
{
    return {v.x * f, v.y * f, v.z * f};
}

Vec3 operator*(float f,const Vec3& v)
{
    return {v.x * f, v.y * f, v.z * f};
}

Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
    return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

bool operator==(const Vec3& v1, const Vec3& v2)
{
    return (v1 - v2).LenSq() < VEC3_EPSILON;
}

bool operator != (const Vec3& v1, const Vec3& v2)
{
    return !(v1 == v2);
}

float Vec3::Dot(const Vec3& v1, const Vec3& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Vec3::LenSq() const
{
    return x * x + y * y + z * z; 
}

float Vec3::Len() const
{
    const float lenSq = LenSq();
    if(lenSq < VEC3_EPSILON)
        return 0.0f;
    
    return std::sqrt(lenSq);
}

void Vec3::Normalize(Vec3& v)
{
    const float lenSq = v.LenSq();
    if(lenSq < VEC3_EPSILON) { return; }

    const float invLen = 1.0f / sqrt(lenSq);
    v.x *= invLen; v.y *= invLen; v.z *= invLen;
}

Vec3 Vec3::Normalized() const
{
    const float lenSq = LenSq();
    if(lenSq < VEC3_EPSILON) { return {x,y,z}; }

    const float invLen = 1.0f / sqrt(lenSq);
    return {x * invLen,y * invLen,z * invLen};
}

float Vec3::Angle(const Vec3& v1, const Vec3& v2)
{
    return acos(Dot(v1, v2) / (v1.Len() * v2.Len()));
}

float Vec3::AngleDeg(const Vec3& v1, const Vec3& v2)
{
    return Angle(v1, v2) * RAD_TO_DEG;
}

Vec3 Vec3::Project(const Vec3& a, const Vec3& b)
{
    const float lenBSq = b.Len();
    if(lenBSq < VEC3_EPSILON) {return {};}

    const float scale = Dot(a,b) / lenBSq;
    return b * scale;
}

Vec3 Vec3::Reject(const Vec3& a, const Vec3& b)
{
    return a - Project(a,b);
}

Vec3 Vec3::Reflect(const Vec3& a, const Vec3& b)
{
    const float magBSq = b.Len();
    if(magBSq < VEC3_EPSILON) { return {}; }

    const float scale = Dot(a,b) / magBSq;
    const Vec3 proj2 = b * (scale * 2);
    return a - proj2;
}

Vec3 Vec3::Cross(const Vec3& v1, const Vec3& v2)
{
    return {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

Vec3 Vec3::Lerp(const Vec3& v1, const Vec3& v2, float t)
{
    return {
        v1.x + (v2.x - v1.x) * t,
        v1.y + (v2.y - v1.y) * t,
        v1.z + (v2.z - v1.z) * t
    };
}

Vec3 Vec3::SLerp(const Vec3& v1, const Vec3& v2, float t)
{
    if(t < 0.01f) { return Lerp(v1,v2,t); }

    const Vec3 from = v1.Normalized();
    const Vec3 to = v2.Normalized();

    const float theta = Angle(from, to);
    const float sinTheta = sin(theta);

    const float a = sin((1.0f - t) * theta) / sinTheta;
    const float b = sin(t * theta) / sinTheta;

    return from * a + to * b;
}

Vec3 Vec3::NLerp(const Vec3& v1, const Vec3& v2, float t)
{
    return Lerp(v1, v2, t).Normalized();
}

float Vec3::Epsilon()
{
    return VEC3_EPSILON;
}
