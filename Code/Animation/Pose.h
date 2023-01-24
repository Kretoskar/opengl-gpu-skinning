#pragma once
#include <vector>

struct Mat4;
struct Transform;

class Pose
{
protected:
    std::vector<Transform> joints;
    std::vector<int> parents;

public:
    Pose();
    Pose(const Pose& p);
    Pose& operator=(const Pose& p);
    Pose(unsigned int numJoints);

    void Resize(unsigned int size);
    unsigned int Size() const;

    int GetParent(unsigned int index) const;
    void SetParent(unsigned int index, int parent);

    Transform GetLocalTransform(unsigned int index) const;
    void SetLocalTransform(unsigned int index, const Transform& transform);
    Transform GetGlobalTransform(unsigned int index);
    Transform operator[](unsigned int index);

    void GetMatrixPalette(std::vector<Mat4>& out);

    bool operator==(const Pose& other);
    bool operator!=(const Pose& other);
};
