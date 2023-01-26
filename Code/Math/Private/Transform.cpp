#include "Math/Public/Transform.h"
#include <cmath>
#include <iostream>

Transform Transform::Combine(const Transform& a, const Transform& b)
{
    Transform out;

    out.scale = a.scale * b.scale;
    out.rotation = b.rotation * a.rotation;

    out.position = a.rotation * (a.scale * b.position);
    out.position = a.position + out.position;

    return out;
}

Transform Transform::Inverse()
{
    Transform inv;

    inv.rotation = (rotation).Inverse();

    inv.scale.x = fabs(scale.x) < VEC3_EPSILON ? 0.0f : 1.0f / scale.x;
    inv.scale.y = fabs(scale.y) < VEC3_EPSILON ? 0.0f : 1.0f / scale.y;
    inv.scale.z = fabs(scale.z) < VEC3_EPSILON ? 0.0f : 1.0f / scale.z;

    Vec3 invTranslation = position * -1.0f;
    inv.position = inv.rotation * (inv.scale * invTranslation);

    return inv;
}

Transform Transform::Mix(const Transform& a, const Transform& b, float t)
{
    Quat bRot = b.rotation;
    if (Quat::Dot(a.rotation, bRot) < 0.0f)
    {
        bRot = -bRot;
    }
    return Transform(
        Vec3::Lerp(a.position, b.position, t),
        Quat::Nlerp(a.rotation, bRot, t),
        Vec3::Lerp(a.scale, b.scale, t));
}

bool operator==(const Transform& a, const Transform& b)
{
    return a.position == b.position &&
        a.rotation == b.rotation &&
        a.scale == b.scale;
}

bool operator!=(const Transform& a, const Transform& b)
{
    return !(a == b);
}

Mat4 Transform::ToMat4()
{
    // First, extract the rotation basis of the transform
    Vec3 x = rotation * Vec3(1, 0, 0);
    Vec3 y = rotation * Vec3(0, 1, 0);
    Vec3 z = rotation * Vec3(0, 0, 1);

    // Next, scale the basis vectors
    x = x * scale.x;
    y = y * scale.y;
    z = z * scale.z;

    // Extract the position of the transform
    Vec3 p = position;

    // Create matrix
    return Mat4(
        x.x, x.y, x.z, 0, // X basis (& Scale)
        y.x, y.y, y.z, 0, // Y basis (& scale)
        z.x, z.y, z.z, 0, // Z basis (& scale)
        p.x, p.y, p.z, 1 // Position
    );
}

Vec3 Transform::TransformPoint(const Vec3& b)
{
    Vec3 out;

    out = rotation * (scale * b);
    out = position + out;

    return out;
}

Vec3 Transform::TransformVector(const Vec3& b)
{
    Vec3 out;

    out = rotation * (scale * b);

    return out;
}
