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
    
    static Quat AngleAxis(float angle, const Vec3& axis);
    static Quat FromTo(const Vec3& from, const Vec3& to);
    static bool SameOrientation(const Quat& a, const Quat& b);
};

Quat operator+(const Quat& a, const Quat& b);
Quat operator-(const Quat& a, const Quat& b);
Quat operator*(const Quat& a, float b);
Quat operator-(const Quat& q);

bool operator==(const Quat& left, const Quat& right);
bool operator!=(const Quat& left, const Quat& right);