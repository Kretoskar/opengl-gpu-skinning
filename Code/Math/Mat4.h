#pragma once

#include <corecrt_math.h>

#include "Quat.h"
#include "Transform.h"
#include "TVec4.h"
#include "Vec3.h"

struct Vec3;

#define MAT4_EPSILON 0.000001f

/**
 * 4x4, column-major matrix with column storage
 */
struct Mat4
{
    union
    {
        float v[16];

        struct
        {
            Vec4 right;
            Vec4 up;
            Vec4 forward;
            Vec4 position;
        };

        struct
        {
            //        row 1     row 2     row 3     row 4
            /*col 1*/ float xx; float xy; float xz; float xw;
            /*col 2*/ float yx; float yy; float yz; float yw;
            /*col 3*/ float zx; float zy; float zz; float zw;
            /*col 4*/ float tx; float ty; float tz; float tw;
        };

        // column-row notation
        struct 
        {
            float c0r0; float c0r1; float c0r2; float c0r3;
            float c1r0; float c1r1; float c1r2; float c1r3;
            float c2r0; float c2r1; float c2r2; float c2r3;
            float c3r0; float c3r1; float c3r2; float c3r3;
        };

        // row-column notation
        struct
        {
            float r0c0; float r1c0; float r2c0; float r3c0;
            float r0c1; float r1c1; float r2c1; float r3c1;
            float r0c2; float r1c2; float r2c2; float r3c2;
            float r0c3; float r1c3; float r2c3; float r3c3;
        };
    };

    // default constructor creates identity matrix
    Mat4() :
        xx(1), xy(0), xz(0), xw(0),
        yx(0), yy(1), yz(0), yw(0),
        zx(0), zy(0), zz(1), zw(0),
        tx(0), ty(0), tz(0), tw(1) {}

    // constructor from float array
    Mat4(float *fv) :
        xx(fv[0]), xy(fv[1]), xz(fv[2]), xw(fv[3]),
        yx(fv[4]), yy(fv[5]), yz(fv[6]), yw(fv[7]),
        zx(fv[8]), zy(fv[9]), zz(fv[10]), zw(fv[11]),
        tx(fv[12]), ty(fv[13]), tz(fv[14]), tw(fv[15]) {}

    Mat4(
        float xx, float xy, float xz, float xw,
        float yx, float yy, float yz, float yw,
        float zx, float zy, float zz, float zw,
        float tx, float ty, float tz, float tw
        
    ) :
        xx(xx), xy(xy), xz(xz), xw(xw),
        yx(yx), yy(yy), yz(yz), yw(yw),
        zx(zx), zy(zy), zz(zz), zw(zw),
        tx(tx), ty(ty), tz(tz), tw(tw) {}

    static void Invert(Mat4& m);

    static void Transpose(Mat4&m);
    
    static Vec3 TransformVector(const Mat4& m, const Vec3& v);
    static Vec3 TransformPoint(const Mat4& m, const Vec3& v);
    static Vec3 TransformPoint(const Mat4& m, const Vec3& v, float &w);
    
    static Mat4 Frustum (float left, float right, float bottom, float top, float near, float far);
    static Mat4 Perspective (float fov, float aspect, float near, float far);
    static Mat4 Orthographic (float left, float right, float bottom, float top, float near, float far);

    static Mat4 LookAt (const Vec3& position, const Vec3& Target, const Vec3& Up);
    
    Mat4 Transposed() const;
    float Determinant() const;
    Mat4 Adjugate() const;
    Mat4 Inverse() const;
    
    Quat ToQuat();
    Transform ToTransform();
};

inline Mat4 operator + (const Mat4& m1, const Mat4& m2)
{
    return {
        m1.xx+m2.xx,    m1.xy+m2.xy,    m1.xz+m2.xz,    m1.xw+m2.xw,
        m1.yx+m2.yx,    m1.yy+m2.yy,    m1.yz+m2.yz,    m1.yw+m2.yw,
        m1.zx+m2.zx,    m1.zy+m2.zy,    m1.zz+m2.zz,    m1.zw+m2.zw,
        m1.tx+m2.tx,    m1.ty+m2.ty,     m1.tz+m2.tz,    m1.tw+m2.tw
    };
}

inline Mat4 operator * (const Mat4& m, float f)
{
    return {
        m.xx*f,     m.xy*f,     m.xz*f,     m.xw*f,
        m.yx*f,     m.yy*f,     m.xz*f,     m.yw*f,
        m.zx*f,     m.zy*f,     m.zz*f,     m.zw*f,
        m.tx*f,     m.ty*f,     m.tz*f,     m.tw*f
    };
}

inline bool operator == (const Mat4& m1, const Mat4& m2)
{
    for(int i = 0; i < 16; ++i)
    {
        if(fabsf(m1.v[i] - m2.v[i]) > MAT4_EPSILON)
        {
            return false;
        }
    }

    return true;
}

inline bool operator != (const Mat4& m1, const Mat4& m2)
{
    return !(m1 == m2);
}

inline Mat4 operator * (float f, const Mat4& m)
{
    return Mat4 (
        m.xx*f,     m.xy*f,     m.xz*f,     m.xw*f,
        m.yx*f,     m.yy*f,     m.yz*f,     m.yw*f,
        m.zx*f,     m.zy*f,     m.zz*f,     m.zw*f,
        m.tx*f,     m.ty*f,     m.tz*f,     m.tw*f
    );
}

#define M4D(m1Row, m2Col)   (\
    m1.v[0 * 4 + (m1Row)] * m2.v[(m2Col) * 4 + 0] + \
    m1.v[1 * 4 + (m1Row)] * m2.v[(m2Col) * 4 + 1] + \
    m1.v[2 * 4 + (m1Row)] * m2.v[(m2Col) * 4 + 2] + \
    m1.v[3 * 4 + (m1Row)] * m2.v[(m2Col) * 4 + 3])

#define M4V4D(mRow, x, y, z, w) \
    ((x)  * m.v[0 * 4 + (mRow)] + \
    (y) * m.v[1 * 4 + (mRow)] + \
    (z) * m.v[2 * 4 + (mRow)] + \
    (w) * m.v[3 * 4 + (mRow)])

inline Vec4 operator * (const Mat4& m, const Vec4& v)
{
    return {
        M4V4D(0, v.x, v.y, v.z, v.w),
        M4V4D(1, v.x, v.y, v.z, v.w),
        M4V4D(2, v.x, v.y, v.z, v.w),
        M4V4D(3, v.x, v.y, v.z, v.w)
    };
}

inline Mat4 operator * (const Mat4& m1, const Mat4& m2)
{
    return {
        M4D(0,0),   M4D(1,0),   M4D(2,0),   M4D(3,0),
        M4D(0,1),   M4D(1,1),   M4D(2,1),   M4D(3,1),
        M4D(0,2),   M4D(1,2),   M4D(2,2),   M4D(3,2),
        M4D(0,3),   M4D(1,3),   M4D(2,3),   M4D(3,3)
    };
}