#pragma once

#include "Math/Public/Vec3.h"
#include "Math/Public/Mat4.h"

#define QUAT_EPSILON 0.000001f

struct Quat
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };

        struct
        {
            Vec3 vector;
            float scalar;
        };

        float v[4];
    };

    Quat() :
        x(0), y(0), z(0), w(1)
    {
    }

    Quat(float _x, float _y, float _z, float _w) :
        x(_x), y(_y), z(_z), w(_w)
    {
    }

    static Quat AngleAxis(float angle, const Vec3& axis);
    static Quat FromTo(const Vec3& from, const Vec3& to);

    static bool SameOrientation(const Quat& left, const Quat& right);
    static float Dot(const Quat& a, const Quat& b);

    static Quat Mix(const Quat& from, const Quat& to, float t);
    static Quat Nlerp(const Quat& from, const Quat& to, float t);

    static Quat Slerp(const Quat& start, const Quat& end, float t);
    static Quat LookRotation(const Vec3& direcion, const Vec3& up);

    Vec3 GetAxis() const;
    float GetAngle() const;
    float LenSq() const;
    float Len() const;
    void Normalize();
    Quat Normalized() const;
    Quat Conjugate() const;
    Quat Inverse() const;
    Mat4 ToMat4();
};

Quat operator+(const Quat& a, const Quat& b);
Quat operator-(const Quat& a, const Quat& b);
Quat operator*(const Quat& a, float b);
Quat operator-(const Quat& q);
bool operator==(const Quat& left, const Quat& right);
bool operator!=(const Quat& a, const Quat& b);

Quat operator*(const Quat& Q1, const Quat& Q2);
Quat operator*(const Quat& Q1, const Quat& Q2);
Vec3 operator*(const Quat& q, const Vec3& v);
Quat operator^(const Quat& q, float f);
