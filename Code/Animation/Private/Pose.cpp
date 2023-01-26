#include "Animation/Public/Pose.h"

Pose::Pose() = default;

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

    if (mParents.size() != p.mParents.size())
    {
        mParents.resize(p.mParents.size());
    }
    if (mJoints.size() != p.mJoints.size())
    {
        mJoints.resize(p.mJoints.size());
    }

    if (!mParents.empty())
    {
        memcpy(&mParents[0], &p.mParents[0],
               sizeof(int) * mParents.size());
    }
    if (!mJoints.empty())
    {
        memcpy(&mJoints[0], &p.mJoints[0],
               sizeof(Transform) * mJoints.size());
    }

    return *this;
}

void Pose::Resize(unsigned int size)
{
    mParents.resize(size);
    mJoints.resize(size);
}

unsigned int Pose::Size() const
{
    return mJoints.size();
}

Transform Pose::GetLocalTransform(unsigned int index) const
{
    return mJoints[index];
}

void Pose::SetLocalTransform(unsigned int index,
                             const Transform& transform)
{
    mJoints[index] = transform;
}

Transform Pose::GetGlobalTransform(unsigned int index) const
{
    Transform result = mJoints[index];
    for (int parent = mParents[index]; parent >= 0;
         parent = mParents[parent])
    {
        result = Transform::Combine(mJoints[parent], result);
    }

    return result;
}

Transform Pose::operator[](unsigned int index)
{
    return GetGlobalTransform(index);
}

void Pose::GetMatrixPalette(std::vector<Mat4>& out) const
{
    const unsigned int size = Size();
    if (out.size() != size)
    {
        out.resize(size);
    }

    for (unsigned int i = 0; i < size; ++i)
    {
        Transform t = GetGlobalTransform(i);
        out[i] = t.ToMat4();
    }
}

int Pose::GetParent(unsigned int index) const
{
    return mParents[index];
}

void Pose::SetParent(unsigned int index, int parent)
{
    mParents[index] = parent;
}

bool Pose::operator==(const Pose& other)
{
    if (mJoints.size() != other.mJoints.size())
    {
        return false;
    }
    if (mParents.size() != other.mParents.size())
    {
        return false;
    }
    unsigned int size = static_cast<unsigned>(mJoints.size());
    for (unsigned int i = 0; i < size; ++i)
    {
        Transform thisLocal = mJoints[i];
        Transform otherLocal = other.mJoints[i];

        int thisParent = mParents[i];
        int otherParent = other.mParents[i];

        if (thisParent != otherParent)
        {
            return false;
        }

        if (thisLocal != otherLocal)
        {
            return false;
        }
    }
    return true;
}

bool Pose::operator!=(const Pose& other)
{
    return !(*this == other);
}
