#include "Mat4.h"
#include "Vec3.h"
#include <corecrt_math.h>
#include <iomanip>
#include <iostream>

 Vec3 Mat4::TransformVector(const Mat4& m, const Vec3& v)
 {
     return {
         M4V4D(0, v.x, v.y, v.z, 0.0f),
        M4V4D(1, v.x, v.y, v.z, 0.0f),
        M4V4D(2, v.x, v.y, v.z, 0.0f)
     };
 }

Vec3 Mat4::TransformPoint(const Mat4& m, const Vec3& v)
{
    return {
        M4V4D(0, v.x, v.y, v.z, 1.0f),
        M4V4D(1, v.x, v.y, v.z, 1.0f),
        M4V4D(2, v.x, v.y, v.z, 1.0f)
    };
}

Vec3 Mat4::TransformPoint(const Mat4& m, const Vec3& v, float& w)
{
    const float _w = w;
    w = M4V4D(3, v.x, v.y, v.z, _w);
    return {
        M4V4D(0, v.x, v.y, v.z, _w),
        M4V4D(1, v.x, v.y, v.z, _w),
        M4V4D(2, v.x, v.y, v.z, _w)
    };
}

#define M4SWAP(x, y) \
    {float t = x; (x) = y; (y) = t;}

void Mat4::Transpose(Mat4& m)
{
    M4SWAP(m.yx, m.xy)
    M4SWAP(m.zx, m.xz)
    M4SWAP(m.tx, m.xw)
    M4SWAP(m.zy, m.yz)
    M4SWAP(m.ty, m.yw)
    M4SWAP(m.tz, m.zw)
}

Mat4 Mat4::Transposed() const
{
    return {
        xx, yx, zx, tx,
        xy, yy, zy, ty,
        xz, yz, zz, tz,
        xw, yw, zw, tw
    };
}

#define M4_3X3MINOR(c0, c1, c2, r0, r1, r2) \
(v[(c0) * 4 + (r0)] * (v[(c1) * 4 + (r1)] * v[(c2) * 4 + (r2)] - v[(c1) * 4 + (r2)] * v[(c2) * 4 + (r1)]) - \
v[(c1) * 4 + (r0)] * (v[(c0) * 4 + (r1)] * v[(c2) * 4 + (r2)] - v[(c0) * 4 + (r2)] * v[(c2) * 4 + (r1)]) + \
v[(c2) * 4 + (r0)] * (v[(c0) * 4 + (r1)] * v[(c1) * 4 + (r2)] - v[(c0) * 4 + (r2)] * v[(c1) * 4 + (r1)]))

float Mat4::Determinant() const {
    return  v[0] * M4_3X3MINOR(1, 2, 3, 1, 2, 3)
        - v[4] * M4_3X3MINOR(0, 2, 3, 1, 2, 3)
        + v[8] * M4_3X3MINOR(0, 1, 3, 1, 2, 3)
        - v[12] * M4_3X3MINOR(0, 1, 2, 1, 2, 3);
}

Mat4 Mat4::Adjugate() const {
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

Mat4 Mat4::Inverse() const {
    const float det = Determinant();

    if (det == 0.0f) { // Epsilon check would need to be REALLY small
        std::cout << "WARNING: Trying to invert a matrix with a zero determinant\n";
        return {};
    }

    const Mat4 adj = Adjugate();

    return adj * (1.0f / det);
}

void Mat4::Invert(Mat4& m) {
    const float det = m.Determinant();

    if (det == 0.0f) {
        std::cout << "WARNING: Trying to invert a matrix with a zero determinant\n";
        m = Mat4();
        return;
    }

    m = m.Adjugate() * (1.0f / det);
}

Mat4 Mat4::Frustum(float left, float right, float bottom, float top, float near, float far)
{
    if (left == right || top == bottom || near == far)
    {
        std::cout << "Invalid frustum\n";
        return {};
    }

    return {
        (2.0f * near) / (right - left), 0, 0, 0,
        0, (2.0f * near) / (top - bottom), 0, 0,
        (right + left) / (right - left), (top + bottom) / (top - bottom), (-(far + near)) / (far - near), -1,
        0, 0, (-2 * far * near) / (far - near), 0
    };
}

Mat4 Mat4::Perspective(float fov, float aspect, float near, float far)
{
    const float yMax = near * tan(fov * 3.14159265359f / 360.0f);
    const float xMax = yMax * aspect;

    return Frustum(-xMax, xMax, -yMax, yMax, near, far);
}

Mat4 Mat4::Orthographic(float left, float right, float bottom, float top, float far, float near)
{
    if (left == right || top == bottom || near == far)
    {
        std::cout << "Invalid frustum\n";
        return {};
    }

    return {
        2.0f / (right - left), 0, 0, 0,
        0, 2.0f / (top - bottom), 0, 0,
        0, 0, -2.0f / (far - near), 0,
        -((right + left)/(right - left)), -((top+bottom)/(top-bottom)), -((far+near) / (far - near)), 1
    }; 
}

Mat4 Mat4::LookAt(const Vec3& position, const Vec3& target, const Vec3& up)
{
    // forward is negative z (wtf opengl)
    const Vec3 forward = (target - position).Normalized() * -1.0f;
    Vec3 right = Vec3::Cross(up, forward);

    // vec3 == already has epsilon comparison
    if(right == Vec3(0,0,0))
    {
        return {};
    }

    Vec3::Normalize(right);
    const Vec3 newUp = Vec3::Cross(forward, right).Normalized();

    const Vec3 t = Vec3(
        -Vec3::Dot(right, position),
        -Vec3::Dot(up, position),
        -Vec3::Dot(forward, position));

    return {
        right.x, newUp.x, forward.x, 0,
        right.y, newUp.y,    forward.y, 0,
        right.z, newUp.z,    forward.z, 0,
        t.x,     t.y,     t.z,        1
    };
}

Quat Mat4::ToQuat()
{
    Vec3 up = Vec3(this->up.x, this->up.y, this->up.z).Normalized();
    const Vec3 forward = Vec3(this->forward.x, this->forward.y, this->forward.z).Normalized();
    const Vec3 right = Vec3::Cross(up, forward);
    up = Vec3::Cross(forward, right);

    return Quat::LookRotation(forward, up);
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
        0, 0, 0, 1);

    Mat4 invRotMat = out.rotation.Inverse().ToMat4();
    Mat4 scaleSkewMat = rotScaleMat * invRotMat;

    out.scale = Vec3(
        scaleSkewMat.v[0],
        scaleSkewMat.v[5],
        scaleSkewMat.v[10]);

    return out;
}
