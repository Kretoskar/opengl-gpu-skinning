#pragma once

struct vec3
{
    // allows data to be accessed either by .x, .y, .z or .v array
    union
    {
        struct 
        {
            float x;
            float y;
            float z;
        };
        float v[3];
    };

    vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    vec3(float *fv) : x(fv[0]), y(fv[1]), z(fv[2]) {}
    
    static float Dot(const vec3 &v1, const vec3 &v2);
    
    static float LenSq(const vec3& v);
    static float Len(const vec3 &v);
    
    static void Normalize (vec3 &v);
    static vec3 Normalized(const vec3 &v);
    
    static float Angle(const vec3 &v1, const vec3 &v2);
    static float AngleDeg(const vec3 &v1, const vec3 &v2);
    
    static vec3 Project(const vec3 &a, const vec3 &b);
    static vec3 Reject(const vec3 &a, const vec3 &b);
    
    static vec3 Reflect(const vec3 &a, const vec3 &b);

    static vec3 Cross(const vec3 &v1, const vec3 &v2);

    static vec3 Lerp(const vec3 &v1, const vec3 &v2, float t);
    static vec3 SLerp(const vec3 &v1, const vec3 &v2, float t);
    static vec3 NLerp(const vec3 &v1, const vec3 &v2, float t);
};