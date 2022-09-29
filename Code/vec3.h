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

    vec3 operator+ (const vec3 &v1, const vec3 &v2);
    vec3 operator- (const vec3 &v1, const vec3 &v2);
    vec3 operator* (const vec3 &v, float f);
    vec3 operator* (const vec3 &v1, const vec3 &v2);
    static float Dot(const vec3 &v1, const vec3 &v2);
    static float LenSq(const vec3& v);
    static float Len(const vec3 &v);
    static void Normalize (vec3 &v);
    static vec3 Normalized(const vec3 &v);
    static float Angle(const vec3 &v1, const vec3 &v2);
    static float AngleDeg(const vec3 &v1, const vec3 &v2);
};