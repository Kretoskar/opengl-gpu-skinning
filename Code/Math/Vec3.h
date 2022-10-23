#pragma once

struct Vec3
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

    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3(float *fv) : x(fv[0]), y(fv[1]), z(fv[2]) {}

    static void Normalize (Vec3 &v);
    
    static float Angle(const Vec3 &v1, const Vec3 &v2);
    static float AngleDeg(const Vec3 &v1, const Vec3 &v2);
    
    static Vec3 Project(const Vec3 &a, const Vec3 &b);
    static Vec3 Reject(const Vec3 &a, const Vec3 &b);
    static Vec3 Reflect(const Vec3 &a, const Vec3 &b);

    static float Dot(const Vec3 &v1, const Vec3 &v2);
    static Vec3 Cross(const Vec3 &v1, const Vec3 &v2);

    static Vec3 Lerp(const Vec3 &v1, const Vec3 &v2, float t);
    static Vec3 SLerp(const Vec3 &v1, const Vec3 &v2, float t);
    static Vec3 NLerp(const Vec3 &v1, const Vec3 &v2, float t);

    static float Epsilon();
    
    float LenSq() const;
    float Len() const;
    
    Vec3 Normalized() const;
};

Vec3 operator+(const Vec3& v1, const Vec3& v2);
Vec3 operator-(const Vec3& v1, const Vec3& v2);
Vec3 operator*(const Vec3& v, float f);
Vec3 operator*(float f,const Vec3& v);
Vec3 operator*(const Vec3& v1, const Vec3& v2);
bool operator==(const Vec3& v1, const Vec3& v2);
bool operator != (const Vec3& v1, const Vec3& v2);