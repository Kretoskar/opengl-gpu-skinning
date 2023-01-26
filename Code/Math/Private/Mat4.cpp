#include "Math/Public/Mat4.h"
#include "Math/Public/Transform.h"
#include <cmath>
#include <iostream>

bool operator==(const Mat4& a, const Mat4& b)
{
    for (int i = 0; i < 16; ++i)
    {
        if (fabsf(a.v[i] - b.v[i]) > MAT4_EPSILON)
        {
            return false;
        }
    }
    return true;
}

bool operator!=(const Mat4& a, const Mat4& b)
{
    return !(a == b);
}

Mat4 operator*(const Mat4& m, float f)
{
    return Mat4(
        m.xx * f, m.xy * f, m.xz * f, m.xw * f,
        m.yx * f, m.yy * f, m.yz * f, m.yw * f,
        m.zx * f, m.zy * f, m.zz * f, m.zw * f,
        m.tx * f, m.ty * f, m.tz * f, m.tw * f
    );
}

Mat4 operator+(const Mat4& a, const Mat4& b)
{
    return Mat4(
        a.xx + b.xx, a.xy + b.xy, a.xz + b.xz, a.xw + b.xw,
        a.yx + b.yx, a.yy + b.yy, a.yz + b.yz, a.yw + b.yw,
        a.zx + b.zx, a.zy + b.zy, a.zz + b.zz, a.zw + b.zw,
        a.tx + b.tx, a.ty + b.ty, a.tz + b.tz, a.tw + b.tw
    );
}

#define M4D(aRow, bCol) \
    a.v[0 * 4 + aRow] * b.v[bCol * 4 + 0] + \
    a.v[1 * 4 + aRow] * b.v[bCol * 4 + 1] + \
    a.v[2 * 4 + aRow] * b.v[bCol * 4 + 2] + \
    a.v[3 * 4 + aRow] * b.v[bCol * 4 + 3]

Mat4 operator*(const Mat4& a, const Mat4& b)
{
    return Mat4(
        M4D(0, 0), M4D(1, 0), M4D(2, 0), M4D(3, 0), // Column 0
        M4D(0, 1), M4D(1, 1), M4D(2, 1), M4D(3, 1), // Column 1
        M4D(0, 2), M4D(1, 2), M4D(2, 2), M4D(3, 2), // Column 2
        M4D(0, 3), M4D(1, 3), M4D(2, 3), M4D(3, 3) // Column 3
    );
}

#define M4V4D(mRow, x, y, z, w) \
    x * m.v[0 * 4 + mRow] + \
    y * m.v[1 * 4 + mRow] + \
    z * m.v[2 * 4 + mRow] + \
    w * m.v[3 * 4 + mRow]

Vec4 operator*(const Mat4& m, const Vec4& v)
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
    {float t = x; x = y; y = t; }

Mat4 Mat4::Frustum(float l, float r, float b, float t, float n, float f)
{
    if (l == r || t == b || n == f)
    {
        std::cout << "WARNING: Trying to create invalid frustum\n";
        return Mat4(); // Error
    }
    return Mat4(
        (2.0f * n) / (r - l), 0, 0, 0,
        0, (2.0f * n) / (t - b), 0, 0,
        (r + l) / (r - l), (t + b) / (t - b), (-(f + n)) / (f - n), -1,
        0, 0, (-2 * f * n) / (f - n), 0
    );
}

Mat4 Mat4::Perspective(float fov, float aspect, float znear, float zfar)
{
    float ymax = znear * tanf(fov * 3.14159265359f / 360.0f);
    float xmax = ymax * aspect;

    return Frustum(-xmax, xmax, -ymax, ymax, znear, zfar);
}

Mat4 Mat4::Ortho(float l, float r, float b, float t, float n, float f)
{
    if (l == r || t == b || n == f)
    {
        return Mat4(); // Error
    }
    return Mat4(
        2.0f / (r - l), 0, 0, 0,
        0, 2.0f / (t - b), 0, 0,
        0, 0, -2.0f / (f - n), 0,
        -((r + l) / (r - l)), -((t + b) / (t - b)), -((f + n) / (f - n)), 1
    );
}

Mat4 Mat4::LookAt(const Vec3& position, const Vec3& target, const Vec3& up)
{
    // Remember, forward is negative z
    Vec3 f = (target - position).Normalized() * -1.0f;
    Vec3 r = Vec3::Cross(up, f); // Right handed
    if (r == Vec3(0, 0, 0))
    {
        return Mat4(); // Error
    }
    r.Normalize();
    Vec3 u = (Vec3::Cross(f, r)).Normalized(); // Right handed

    auto t = Vec3(
        -Vec3::Dot(r, position),
        -Vec3::Dot(u, position),
        -Vec3::Dot(f, position)
    );

    return Mat4(
        // Transpose upper 3x3 matrix to invert it
        r.x, u.x, f.x, 0,
        r.y, u.y, f.y, 0,
        r.z, u.z, f.z, 0,
        t.x, t.y, t.z, 1
    );
}

void Mat4::Transpose()
{
    M4SWAP(yx, xy);
    M4SWAP(zx, xz);
    M4SWAP(tx, xw);
    M4SWAP(zy, yz);
    M4SWAP(ty, yw);
    M4SWAP(tz, zw);
}

Mat4 Mat4::Transposed()
{
    return Mat4(
        xx, yx, zx, tx,
        xy, yy, zy, ty,
        xz, yz, zz, tz,
        xw, yw, zw, tw
    );
}

#define M4_3X3MINOR(c0, c1, c2, r0, r1, r2) \
    (v[c0 * 4 + r0] * (v[c1 * 4 + r1] * v[c2 * 4 + r2] - v[c1 * 4 + r2] * v[c2 * 4 + r1]) - \
     v[c1 * 4 + r0] * (v[c0 * 4 + r1] * v[c2 * 4 + r2] - v[c0 * 4 + r2] * v[c2 * 4 + r1]) + \
     v[c2 * 4 + r0] * (v[c0 * 4 + r1] * v[c1 * 4 + r2] - v[c0 * 4 + r2] * v[c1 * 4 + r1]))

float Mat4::Determinant()
{
    return v[0] * M4_3X3MINOR(1, 2, 3, 1, 2, 3)
        - v[4] * M4_3X3MINOR(0, 2, 3, 1, 2, 3)
        + v[8] * M4_3X3MINOR(0, 1, 3, 1, 2, 3)
        - v[12] * M4_3X3MINOR(0, 1, 2, 1, 2, 3);
}

Mat4 Mat4::Adjugate()
{
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

    return cofactor.Transposed();
}

Mat4 Mat4::Inverse()
{
    float det = Determinant();

    if (det == 0.0f)
    {
        // Epsilon check would need to be REALLY small
        std::cout << "WARNING: Trying to invert a matrix with a zero determinant\n";
        return Mat4();
    }
    Mat4 adj = Adjugate();

    return adj * (1.0f / det);
}

void Mat4::Invert(Mat4& m)
{
    float det = m.Determinant();

    if (det == 0.0f)
    {
        std::cout << "WARNING: Trying to invert a matrix with a zero determinant\n";
        m = Mat4();
        return;
    }

    m = m.Adjugate() * (1.0f / det);
}

Transform Mat4::ToTransform()
{
    Transform out;

    out.position = Vec3(v[12], v[13], v[14]);
    out.rotation = ToQuat();

    Mat4 rotScaleMat(
        v[0], v[1], v[2], 0,
        v[4], v[5], v[6], 0,
        v[8], v[9], v[10], 0,
        0, 0, 0, 1
    );
    Mat4 invRotMat = ((out.rotation).Inverse()).ToMat4();
    Mat4 scaleSkewMat = rotScaleMat * invRotMat;

    out.scale = Vec3(
        scaleSkewMat.v[0],
        scaleSkewMat.v[5],
        scaleSkewMat.v[10]
    );

    return out;
}

Quat Mat4::ToQuat()
{
    Vec3 u = (Vec3(up.x, up.y, up.z)).Normalized();
    Vec3 f = (Vec3(forward.x, forward.y, forward.z)).Normalized();
    Vec3 r = Vec3::Cross(u, f);
    u = Vec3::Cross(f, r);

    return Quat::LookRotation(f, u);
}
