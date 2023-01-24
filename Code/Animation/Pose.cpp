#include "Pose.h"

#include "Math/Mat4.h"
#include "Math/Transform.h"

Pose::Pose() { }

Pose::Pose(unsigned int numJoints)
{
    Resize(numJoints);
}

Pose::Pose(const Pose& p)
{
    *this = p;
}

Pose& Pose::operator=(const Pose& p)
{
    if (&p == this)
    {
        return *this;
    }
    
    if (parents.size() != p.parents.size())
    {
        parents.resize(p.parents.size());
    }
    
    if (joints.size() != p.joints.size())
    {
        joints.resize(p.joints.size());
    }

    if (!parents.empty())
    {
        memcpy(&parents[0], &p.parents[0], 
               sizeof(int) * parents.size());
    }
    if (!joints.empty())
    {
        memcpy(&joints[0], &p.joints[0], 
               sizeof(Transform) * joints.size());
    }
    
    return *this;
}

void Pose::Resize(unsigned int size)
{
    parents.resize(size);
    joints.resize(size);
}

unsigned int Pose::Size() const
{
    return joints.size();
}

Transform Pose::GetLocalTransform(unsigned int index) const
{
    return joints[index];
}

void Pose::SetLocalTransform(unsigned int index, const Transform& transform)
{
    joints[index] = transform;
}

Transform Pose::GetGlobalTransform(unsigned int i)
{
    Transform result = joints[i];

    for (int p = parents[i]; p >= 0; p = parents[p])
    {
        result = Transform::Combine(joints[p], result);
    }

    return result;
}

Transform Pose::operator[](unsigned int index)
{
    return GetGlobalTransform(index);
}

void Pose::GetMatrixPalette(std::vector<Mat4>& out)
{
    unsigned int size = Size();

    if (out.size() != size)
    {
        out.resize(size);
    }

    for (unsigned int i = 0; i < size; ++i)
    {
        Transform t = GetGlobalTransform(i);
        out[i] =  t.ToMat4();
    }
}

int Pose::GetParent(unsigned int index) const
{
    return parents[index];
}

void Pose::SetParent(unsigned int index, int parent)
{
    parents[index] = parent;
}

bool Pose::operator==(const Pose& other)
{
    if (joints.size() != other.joints.size())
    {
        return false;
    }
    
    if (parents.size() != other.parents.size())
    {
        return false;
    }

    unsigned int size = static_cast<unsigned int>(joints.size());

    for (unsigned int i = 0; i < size; ++i)
    {
        Transform thisLocal = joints[i];
        Transform otherLocal = other.joints[i];

        const int thisParent = parents[i];
        const int otherParent = other.parents[i];

        if (thisParent != otherParent)
        {
            return false;
        }

        if (thisLocal.position != otherLocal.position)
        {
            return false;
        }

        if (thisLocal.rotation != otherLocal.rotation)
        {
            return false;
        }

        if (thisLocal.scale != otherLocal.scale)
        { 
            return false;
        } 
    }
    return true;
}
bool Pose::operator!=(const Pose& other) {
    return !(*this == other);
}