#include "vec3.h"
#include <corecrt_math.h>

#define VEC3_EPSILON 0.000001f
#define RAD_TO_DEG 57.2958f

vec3 vec3::operator+(const vec3& v1, const vec3& v2)
{
    return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

vec3 vec3::operator-(const vec3& v1, const vec3& v2)
{
    return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

vec3 vec3::operator*(const vec3& v, float f)
{
    return vec3(v.x * f, v.y * f, v.z * f);
}

vec3 vec3::operator*(const vec3& v1, const vec3& v2)
{
    return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

float vec3::Dot(const vec3& v1, const vec3& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float vec3::LenSq(const vec3& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z; 
}

float vec3::Len(const vec3& v)
{
    float lenSq = LenSq(v);

    if(lenSq < VEC3_EPSILON)
    {
        return 0.0f;
    }

    return sqrtf(lenSq);
}

void vec3::Normalize(vec3& v)
{
    float lenSq = LenSq(v);
    if(lenSq < VEC3_EPSILON) { return; }
    float invLen = 1.0f / sqrtf(lenSq);
    v.x *= invLen;
    v.y *= invLen;
    v.z *= invLen;
}

vec3 vec3::Normalized(const vec3& v)
{
    float lenSq = LenSq(v);
    if(lenSq < VEC3_EPSILON) { return v; }
    float invLen = 1.0f / sqrtf(lenSq);
    return vec3(
        v.x * invLen,
        v.y * invLen,
        v.z * invLen
    );
}

float vec3::Angle(const vec3& v1, const vec3& v2)
{
    return acosf(Dot(v1, v2) / (Len(v1) * Len(v2)));
}

float vec3::AngleDeg(const vec3& v1, const vec3& v2)
{
    return Angle(v1, v2) * RAD_TO_DEG;
}


