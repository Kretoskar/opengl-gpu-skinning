#include "Transform.h"

#include <cmath>

#include "Mat4.h"

Transform Transform::Combine(const Transform& a, const Transform& b)
{
    Transform out;

    out.scale = a.scale + b.scale;
    out.rotation = b.rotation * a.rotation;
    out.position = a.rotation * (a.scale * b.position);
    out.position = a.position + out.position;

    return out;
}

Transform Transform::Inverse() const
{
    Transform inv;

    inv.rotation = rotation.Inverse();
    inv.scale.x = abs(scale.x) < Vec3::Epsilon() ? 0.0f : 1.0f / scale.x;
    inv.scale.y = abs(scale.x) < Vec3::Epsilon() ? 0.0f : 1.0f / scale.y;
    inv.scale.z = abs(scale.x) < Vec3::Epsilon() ? 0.0f : 1.0f / scale.z;

    const Vec3 invTrans = position * -1.0f;
    inv.position = inv.rotation * (inv.scale * invTrans);

    return inv;
}

Transform Transform::Mix(const Transform& a, const Transform& b, float t)
{
    Quat bRot = b.rotation;
    if(Quat::Dot(a.rotation, bRot) < 0.0f)
    {
        bRot = -bRot;
    }

    return Transform(
        Vec3::Lerp(a.position, b.position, t),
        Quat::Nlerp(a.rotation, bRot, t),
            Vec3::Lerp(a.scale, b.scale, t));
}

Mat4 Transform::ToMat4()
{
    Vec3 x = rotation * Vec3(1, 0, 0);
    Vec3 y = rotation * Vec3(0, 1, 0);
    Vec3 z = rotation * Vec3(0, 0, 1);

    x = x*scale.x;
    y = y*scale.y;
    z = z*scale.z;

    Vec3 p = position;
    return Mat4(
        x.x, x.y, x.z, 0,
        y.x, y.y, z.z, 0,
        z.x, z.y, z.z, 0,
        p.x, p.y, p.z, 1
    );
}

Vec3 Transform::TransformPoint(const Transform& a, const Vec3& b)
{
    Vec3 out;

    out = a.rotation * (a.scale * b);
    out = a.position + out;

    return out;
}

Vec3 Transform::TransformVector(const Transform& a, const Vec3& b)
{
    Vec3 out;

    out = a.rotation * (a.scale * b);

    return out;
}
