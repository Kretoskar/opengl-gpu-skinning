#pragma once
#include "Quat.h"
#include "TVec4.h"
#include "Vec3.h"

struct Vec3;

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

    Mat4 Transposed() const;
    float Determinant() const;
    Mat4 Adjugate() const;

    Mat4 Inverse() const;
    
    static void Invert(Mat4& m);

    static void Transpose(Mat4&m);
    
    static Vec3 TransformVector(const Mat4& m, const Vec3& v);
    static Vec3 TransformPoint(const Mat4& m, const Vec3& v);
    static Vec3 TransformPoint(const Mat4& m, const Vec3& v, float &w);
    
    static Mat4 Frustum (float left, float right, float bottom, float top, float near, float far);
    static Mat4 Perspective (float fov, float aspect, float near, float far);
    static Mat4 Orthographic (float left, float right, float bottom, float top, float near, float far);

    static Mat4 LookAt (const Vec3& position, const Vec3& Target, const Vec3& Up);

    Quat ToQuat();
};
