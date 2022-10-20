#pragma once

#include "Vec3.h"

struct Mat4;

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

    Quat() : x(0), y(0), z(0), w(1) {}
    Quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    Vec3 GetAxis (const Quat& quat);
    float GetAngle (const Quat& quat);

    Quat Normalized() const;
    
    static void Normalize(Quat&q);
    static Quat AngleAxis(float angle, const Vec3& axis);
    static Quat FromTo(const Vec3& from, const Vec3& to);
    static bool SameOrientation(const Quat& a, const Quat& b);

    static float Dot(const Quat& a, const Quat& b);

    float LenSq() const;
    float Len() const;

    Quat Conjugate();
    Quat Inverse() const;
    
    //Lerp, assuming that input quaternions are in the desired neighborhood
    Quat Mix(const Quat& from, const Quat& to, float t);
    //Assums that input quaternions are in the desired neighborhood
    Quat Nlerp(const Quat& from, const Quat& to, float t);
    Quat Slerp(const Quat& start, const Quat& end, float t);

    static Quat LookRotation(const Vec3& direction, const Vec3& refUp);

    Mat4 ToMat4() const;
};

Quat operator+(const Quat& a, const Quat& b);
Quat operator-(const Quat& a, const Quat& b);
Quat operator*(const Quat& a, float b);
Quat operator-(const Quat& q);
Quat operator*(const Quat& a, const Quat&b);
Vec3 operator*(const Quat& q, const Vec3& v);
Quat operator^(const Quat& q, float f);

bool operator==(const Quat& left, const Quat& right);
bool operator!=(const Quat& left, const Quat& right);