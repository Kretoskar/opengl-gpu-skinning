#pragma once

#define VEC3_EPSILON 0.000001f

struct Vec3
{
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

    Vec3() : x(0.0f), y(0.0f), z(0.0f)
    {
    }

    Vec3(float _x, float _y, float _z) :
        x(_x), y(_y), z(_z)
    {
    }

    Vec3(float* fv) :
        x(fv[0]), y(fv[1]), z(fv[2])
    {
    }

    static float Dot(const Vec3& l, const Vec3& r);
    static Vec3 Cross(const Vec3& l, const Vec3& r);
    static float Angle(const Vec3& l, const Vec3& r);

    static Vec3 Project(const Vec3& a, const Vec3& b);
    static Vec3 Reject(const Vec3& a, const Vec3& b);
    static Vec3 Reflect(const Vec3& a, const Vec3& b);

    static Vec3 Lerp(const Vec3& s, const Vec3& e, float t);
    static Vec3 Slerp(const Vec3& s, const Vec3& e, float t);
    static Vec3 Nlerp(const Vec3& s, const Vec3& e, float t);


    float LenSq() const;
    float Len() const;
    void Normalize();
    Vec3 Normalized() const;
};

Vec3 operator+(const Vec3& l, const Vec3& r);
Vec3 operator-(const Vec3& l, const Vec3& r);
Vec3 operator*(const Vec3& v, float f);
Vec3 operator*(const Vec3& l, const Vec3& r);
bool operator==(const Vec3& l, const Vec3& r);
bool operator!=(const Vec3& l, const Vec3& r);
