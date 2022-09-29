#include "Vec3.h"
#include <corecrt_math.h>

#define VEC3_EPSILON 0.000001f
#define RAD_TO_DEG 57.2958f

Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vec3 operator*(const Vec3& v, float f)
{
    return Vec3(v.x * f, v.y * f, v.z * f);
}

Vec3 operator*(float f,const Vec3& v)
{
    return Vec3(v.x * f, v.y * f, v.z * f);
}

Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

bool operator==(const Vec3& v1, const Vec3& v2)
{
    return Vec3::LenSq(v1 - v2) < VEC3_EPSILON;
}

bool operator != (const Vec3& v1, const Vec3& v2)
{
    return !(v1 == v2);
}

float Vec3::Dot(const Vec3& v1, const Vec3& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Vec3::LenSq(const Vec3& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z; 
}

float Vec3::Len(const Vec3& v)
{
    float lenSq = LenSq(v);
    if(lenSq < VEC3_EPSILON)
        return 0.0f;
    
    return sqrtf(lenSq);
}

void Vec3::Normalize(Vec3& v)
{
    float lenSq = LenSq(v);
    if(lenSq < VEC3_EPSILON) { return; }
    
    float invLen = 1.0f / sqrtf(lenSq);
    v.x *= invLen; v.y *= invLen; v.z *= invLen;
}

Vec3 Vec3::Normalized(const Vec3& v)
{
    float lenSq = LenSq(v);
    if(lenSq < VEC3_EPSILON) { return v; }
    
    float invLen = 1.0f / sqrtf(lenSq);
    return Vec3(v.x * invLen,v.y * invLen,v.z * invLen);
}

float Vec3::Angle(const Vec3& v1, const Vec3& v2)
{
    return acosf(Dot(v1, v2) / (Len(v1) * Len(v2)));
}

float Vec3::AngleDeg(const Vec3& v1, const Vec3& v2)
{
    return Angle(v1, v2) * RAD_TO_DEG;
}

Vec3 Vec3::Project(const Vec3& a, const Vec3& b)
{
    float lenBSq = Len(b);
    if(lenBSq < VEC3_EPSILON) {return Vec3();}

    float scale = Dot(a,b) / lenBSq;
    return b * scale;
}

Vec3 Vec3::Reject(const Vec3& a, const Vec3& b)
{
    return a - Project(a,b);
}

Vec3 Vec3::Reflect(const Vec3& a, const Vec3& b)
{
    float magBSq = Len(b);
    if(magBSq < VEC3_EPSILON) { return Vec3(); }

    float scale = Dot(a,b) / magBSq;
    Vec3 proj2 = b * (scale * 2);
    return a - proj2;
}

Vec3 Vec3::Cross(const Vec3& v1, const Vec3& v2)
{
    return Vec3(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

Vec3 Vec3::Lerp(const Vec3& v1, const Vec3& v2, float t)
{
    return Vec3(
        v1.x + (v2.x - v1.x) * t,
        v1.y + (v2.y - v1.y) * t,
        v1.z + (v2.z - v1.z) * t
    );
}

Vec3 Vec3::SLerp(const Vec3& v1, const Vec3& v2, float t)
{
    if(t < 0.01f) { return Lerp(v1,v2,t); }

    Vec3 from = Normalized(v1);
    Vec3 to = Normalized(v2);

    float theta = Angle(from, to);
    float sinTheta = sinf(theta);

    float a = sinf((1.0f - t) * theta) / sinTheta;
    float b = sinf(t * theta) / sinTheta;

    return from * a + to * b;
}

Vec3 Vec3::NLerp(const Vec3& v1, const Vec3& v2, float t)
{
    return Normalized(Lerp(v1, v2, t));
}
