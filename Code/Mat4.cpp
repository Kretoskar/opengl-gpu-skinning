#include "Mat4.h"
#include <corecrt_math.h>
#include <iostream>

#define MAT4_EPSILON 0.000001f

bool operator == (const Mat4& m1, const Mat4& m2)
{
    for(int i = 0; i < 16; i++)
    {
        if(fabsf(m1.v[i] - m2.v[i]) > MAT4_EPSILON)
        {
            return false;
        }
    }

    return true;
}

bool operator != (const Mat4& m1, const Mat4& m2)
{
    return !(m1 == m2);
}

Mat4 operator + (const Mat4& m1, const Mat4& m2)
{
    return Mat4 (
        m1.xx+m2.xx,    m1.xy+m2.xy,    m1.xz+m2.xz,    m1.xw+m2.xw,
        m1.yx+m2.yx,    m1.yy+m2.yy,    m1.yz+m2.yz,    m1.yw+m2.yw,
        m1.zx+m2.zx,    m1.zy+m2.zy,    m1.zz+m2.zz,    m1.zw+m2.zw,
        m1.tx+m2.tx,    m1.ty+m2.ty,     m1.tz+m2.tz,    m1.tw+m2.tw
    );
}

Mat4 operator * (const Mat4& m, float f)
{
    return Mat4 (
        m.xx*f,     m.xy*f,     m.xz*f,     m.xw*f,
        m.yx*f,     m.yy*f,     m.xz*f,     m.yw*f,
        m.zx*f,     m.zy*f,     m.zz*f,     m.zw*f,
        m.tx*f,     m.ty*f,     m.tz*f,     m.tw*f
    );
}

Mat4 operator * (float f, const Mat4& m)
{
    return Mat4 (
        m.xx*f,     m.xy*f,     m.xz*f,     m.xw*f,
        m.yx*f,     m.yy*f,     m.xz*f,     m.yw*f,
        m.zx*f,     m.zy*f,     m.zz*f,     m.zw*f,
        m.tx*f,     m.ty*f,     m.tz*f,     m.tw*f
    );
}

#define M4D(m1Row, m2Col)   \
m1.v[0 * 4 + m1Row] * m2.v[m2Col * 4 + 0] + \
m1.v[1 * 4 + m1Row] * m2.v[m2Col * 4 + 1] + \
m1.v[2 * 4 + m1Row] * m2.v[m2Col * 4 + 2] + \
m1.v[3 * 4 + m1Row] * m2.v[m2Col * 4 + 3]

Mat4 operator * (const Mat4& m1, const Mat4& m2)
{
    return Mat4(
        M4D(0,0),   M4D(1,0),   M4D(2,0),   M4D(3,0),
        M4D(0,1),   M4D(1,1),   M4D(2,1),   M4D(3,1),
        M4D(0,2),   M4D(1,2),   M4D(2,2),   M4D(3,2),
        M4D(0,3),   M4D(1,3),   M4D(2,3),   M4D(3,3)
    );
}

#define M4V4D(mRow, x, y, z, w) \
    x * m.v[0 * 4 + mRow] + \
    y * m.v[1 * 4 + mRow] + \
    z * m.v[2 * 4 + mRow] + \
    w * m.v[3 * 4 + mRow]

Vec4 operator * (const Mat4& m, const Vec4& v)
{
    return Vec4(
        M4V4D(0, v.x, v.y, v.z, v.w),
        M4V4D(1, v.x, v.y, v.z, v.w),
        M4V4D(2, v.x, v.y, v.z, v.w),
        M4V4D(3, v.x, v.y, v.z, v.w)
    );
}
 Vec3 Mat4::TransformVector(const Mat4& m, const Vec3& v)
 {
     return Vec3(
        M4V4D(0, v.x, v.y, v.z, 0.0f),
        M4V4D(1, v.x, v.y, v.z, 0.0f),
        M4V4D(2, v.x, v.y, v.z, 0.0f)
     );
 }

Vec3 Mat4::TransformPoint(const Mat4& m, const Vec3& v)
{
    return Vec3(
        M4V4D(0, v.x, v.y, v.z, 1.0f),
        M4V4D(1, v.x, v.y, v.z, 1.0f),
        M4V4D(2, v.x, v.y, v.z, 1.0f)
     );
}

Vec3 Mat4::TransformPoint(const Mat4& m, const Vec3& v, float& w)
{
    float _w = w;
    w = M4V4D(3, v.x, v.y, v.z, _w);
    return Vec3(
        M4V4D(0, v.x, v.y, v.z, _w),
        M4V4D(1, v.x, v.y, v.z, _w),
        M4V4D(2, v.x, v.y, v.z, _w)
     );
}

#define M4SWAP(x, y) \
    {float t = x; x = y; y = t;}

void Mat4::Transpose(Mat4& m)
{
    M4SWAP(m.yx, m.xy)
    M4SWAP(m.zx, m.xz)
    M4SWAP(m.tx, m.xw)
    M4SWAP(m.zy, m.yz)
    M4SWAP(m.ty, m.yw)
    M4SWAP(m.tz, m.zw)
}

Mat4 Mat4::Transposed(const Mat4& m)
{
    return Mat4(
        m.xx, m.yx, m.zx, m.tx,
        m.xy, m.yy, m.zy, m.ty,
        m.xz, m.yz, m.zz, m.tz,
        m.xw, m.yw, m.zw, m.tw
    );
}

#define M4_3X3MINOR(c0, c1, c2, r0, r1, r2) \
(m.v[c0 * 4 + r0] * (m.v[c1 * 4 + r1] * m.v[c2 * 4 + r2] - m.v[c1 * 4 + r2] * m.v[c2 * 4 + r1]) - \
m.v[c1 * 4 + r0] * (m.v[c0 * 4 + r1] * m.v[c2 * 4 + r2] - m.v[c0 * 4 + r2] * m.v[c2 * 4 + r1]) + \
m.v[c2 * 4 + r0] * (m.v[c0 * 4 + r1] * m.v[c1 * 4 + r2] - m.v[c0 * 4 + r2] * m.v[c1 * 4 + r1]))

float Mat4::Determinant(const Mat4& m) {
    return  m.v[0] * M4_3X3MINOR(1, 2, 3, 1, 2, 3)
        - m.v[4] * M4_3X3MINOR(0, 2, 3, 1, 2, 3)
        + m.v[8] * M4_3X3MINOR(0, 1, 3, 1, 2, 3)
        - m.v[12] * M4_3X3MINOR(0, 1, 2, 1, 2, 3);
}

Mat4 Mat4::Adjugate(const Mat4& m) {
    // Cofactor(M[i, j]) = Minor(M[i, j]] * pow(-1, i + j)
    Mat4 cofactor;

    cofactor.v[0] = M4_3X3MINOR(1, 2, 3, 1, 2, 3);
    cofactor.v[1] = -M4_3X3MINOR(1, 2, 3, 0, 2, 3);
    cofactor.v[2] = M4_3X3MINOR(1, 2, 3, 0, 1, 3);
    cofactor.v[3] = -M4_3X3MINOR(1, 2, 3, 0, 1, 2);

    cofactor.v[4] = -M4_3X3MINOR(0, 2, 3, 1, 2, 3);
    cofactor.v[5] = M4_3X3MINOR(0, 2, 3, 0, 2, 3);
    cofactor.v[6] = -M4_3X3MINOR(0, 2, 3, 0, 1, 3);
    cofactor.v[7] = M4_3X3MINOR(0, 2, 3, 0, 1, 2);

    cofactor.v[8] = M4_3X3MINOR(0, 1, 3, 1, 2, 3);
    cofactor.v[9] = -M4_3X3MINOR(0, 1, 3, 0, 2, 3);
    cofactor.v[10] = M4_3X3MINOR(0, 1, 3, 0, 1, 3);
    cofactor.v[11] = -M4_3X3MINOR(0, 1, 3, 0, 1, 2);

    cofactor.v[12] = -M4_3X3MINOR(0, 1, 2, 1, 2, 3);
    cofactor.v[13] = M4_3X3MINOR(0, 1, 2, 0, 2, 3);
    cofactor.v[14] = -M4_3X3MINOR(0, 1, 2, 0, 1, 3);
    cofactor.v[15] = M4_3X3MINOR(0, 1, 2, 0, 1, 2);

    return Transposed(cofactor);
}

Mat4 Mat4::Inverse(const Mat4& m) {
    float det = Determinant(m);

    if (det == 0.0f) { // Epsilon check would need to be REALLY small
        std::cout << "WARNING: Trying to invert a matrix with a zero determinant\n";
        return Mat4();
    }
    
    Mat4 adj = Adjugate(m);

    return adj * (1.0f / det);
}

void Mat4::Invert(Mat4& m) {
    float det = Determinant(m);

    if (det == 0.0f) {
        std::cout << "WARNING: Trying to invert a matrix with a zero determinant\n";
        m = Mat4();
        return;
    }

    m = Adjugate(m) * (1.0f / det);
}

Mat4 Mat4::Frustum(float left, float right, float bottom, float top, float near, float far)
{
    if (left == right || top == bottom || near == far)
    {
        std::cout << "Invalid frustum\n";
        return Mat4();
    }

    return Mat4(
        (2.0f * near) / (right - 1), 0, 0, 0,
        0, (2.0f * near) / (top - bottom), 0, 0,
        (right + 1) / (right - 1), (top + bottom) / (top - bottom), (-(far + near)) / (far - near), -1,
        0, 0, (-2 * far * near) / (far - near), 0);
}

Mat4 Mat4::Perspective(float fov, float aspect, float near, float far)
{
    float yMax = near * tanf(fov * 3.14159265359f / 360.0f);
    float xMax = yMax * aspect;

    return Frustum(-xMax, xMax, -yMax, yMax, near, far);
}

Mat4 Mat4::Orthographic(float left, float right, float bottom, float top, float far, float near)
{
    if (left == right || top == bottom || near == far)
    {
        std::cout << "Invalid frustum\n";
        return Mat4();
    }

    return Mat4(
        2.0f / (right - 1), 0, 0, 0,
        0, 2.0f / (top - bottom), 0, 0,
        0, 0, -2.0f / (far - near), 0,
        -((right + 1)/(right - 1)), -((top+bottom)/(top-bottom)), -((far+near) / (far - near)), 1); 
}
