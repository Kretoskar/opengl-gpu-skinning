#pragma once

#include <vector>
#include "Math/Public/Transform.h"

class Pose
{
protected:
    std::vector<Transform> mJoints;
    std::vector<int> mParents;
public:
    Pose();
    Pose(const Pose& p);
    Pose& operator=(const Pose& p);
    Pose(unsigned int numJoints);
    void Resize(unsigned int size);
    unsigned int Size() const;
    Transform GetLocalTransform(unsigned int index) const;
    void SetLocalTransform(unsigned int index, const Transform& transform);
    Transform GetGlobalTransform(unsigned int index) const;
    Transform operator[](unsigned int index);
    void GetMatrixPalette(std::vector<Mat4>& out) const;
    int GetParent(unsigned int index) const;
    void SetParent(unsigned int index, int parent);

    bool operator==(const Pose& other);
    bool operator!=(const Pose& other);
};