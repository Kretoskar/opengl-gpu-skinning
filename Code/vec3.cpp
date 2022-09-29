#include "vec3.h"
#include <corecrt_math.h>

#define VEC3_EPSILON 0.000001f
#define RAD_TO_DEG 57.2958f

vec3 operator+(const vec3& v1, const vec3& v2)
{
    return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

vec3 operator-(const vec3& v1, const vec3& v2)
{
    return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

vec3 operator*(const vec3& v, float f)
{
    return vec3(v.x * f, v.y * f, v.z * f);
}

vec3 operator*(float f,const vec3& v)
{
    return vec3(v.x * f, v.y * f, v.z * f);
}

vec3 operator*(const vec3& v1, const vec3& v2)
{
    return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

bool operator==(const vec3& v1, const vec3& v2)
{
    return vec3::LenSq(v1 - v2) < VEC3_EPSILON;
}

bool operator != (const vec3& v1, const vec3& v2)
{
    return !(v1 == v2);
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
        return 0.0f;
    
    return sqrtf(lenSq);
}

void vec3::Normalize(vec3& v)
{
    float lenSq = LenSq(v);
    if(lenSq < VEC3_EPSILON) { return; }
    
    float invLen = 1.0f / sqrtf(lenSq);
    v.x *= invLen; v.y *= invLen; v.z *= invLen;
}

vec3 vec3::Normalized(const vec3& v)
{
    float lenSq = LenSq(v);
    if(lenSq < VEC3_EPSILON) { return v; }
    
    float invLen = 1.0f / sqrtf(lenSq);
    return vec3(v.x * invLen,v.y * invLen,v.z * invLen);
}

float vec3::Angle(const vec3& v1, const vec3& v2)
{
    return acosf(Dot(v1, v2) / (Len(v1) * Len(v2)));
}

float vec3::AngleDeg(const vec3& v1, const vec3& v2)
{
    return Angle(v1, v2) * RAD_TO_DEG;
}

vec3 vec3::Project(const vec3& a, const vec3& b)
{
    float lenBSq = Len(b);
    if(lenBSq < VEC3_EPSILON) {return vec3();}

    float scale = Dot(a,b) / lenBSq;
    return b * scale;
}

vec3 vec3::Reject(const vec3& a, const vec3& b)
{
    return a - Project(a,b);
}

vec3 vec3::Reflect(const vec3& a, const vec3& b)
{
    float magBSq = Len(b);
    if(magBSq < VEC3_EPSILON) { return vec3(); }

    float scale = Dot(a,b) / magBSq;
    vec3 proj2 = b * (scale * 2);
    return a - proj2;
}

vec3 vec3::Cross(const vec3& v1, const vec3& v2)
{
    return vec3(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

vec3 vec3::Lerp(const vec3& v1, const vec3& v2, float t)
{
    return vec3(
        v1.x + (v2.x - v1.x) * t,
        v1.y + (v2.y - v1.y) * t,
        v1.z + (v2.z - v1.z) * t
    );
}

vec3 vec3::SLerp(const vec3& v1, const vec3& v2, float t)
{
    if(t < 0.01f) { return Lerp(v1,v2,t); }

    vec3 from = Normalized(v1);
    vec3 to = Normalized(v2);

    float theta = Angle(from, to);
    float sinTheta = sinf(theta);

    float a = sinf((1.0f - t) * theta) / sinTheta;
    float b = sinf(t * theta) / sinTheta;

    return from * a + to * b;
}

vec3 vec3::NLerp(const vec3& v1, const vec3& v2, float t)
{
    return Normalized(Lerp(v1, v2, t));
}
