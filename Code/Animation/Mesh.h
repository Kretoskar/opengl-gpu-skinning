#pragma once
#include <vector>

#include "Skeleton.h"
#include "Math/Mat4.h"
#include "Math/TVec2.h"
#include "Math/TVec4.h"
#include "Math/Vec3.h"
#include "Rendering/Attribute.h"
#include "Rendering/IndexBuffer.h"

class Mesh
{
protected:
    std::vector<Vec3> mPosition;
    std::vector<Vec3> mNormal;
    std::vector<Vec2> mTexCoord;
    std::vector<Vec4> mWeights;
    std::vector<IVec4> mInfluences;
    std::vector<unsigned int> mIndices;

    Attribute<Vec3>* mPosAttrib;
    Attribute<Vec3>* mNormAttrib;
    Attribute<Vec2>* mUvAttrib;
    Attribute<Vec4>* mWeightAttrib;
    Attribute<IVec4>* mInfluenceAttrib;
    IndexBuffer* mIndexBuffer;

    std::vector<Vec3> mSkinnedPosition;
    std::vector<Vec3> mSkinnedNormal;
    std::vector<Mat4> mPosePalette;

public:
    Mesh();
    Mesh(const Mesh&);
    Mesh& operator=(const Mesh&);
    ~Mesh();

    std::vector<Vec3>& GetPosition();
    std::vector<Vec3>& GetNormal();
    std::vector<Vec2>& GetTexCoord();
    std::vector<Vec4>& GetWeights();
    std::vector<IVec4>& GetInfluences();
    std::vector<unsigned int>& GetIndices();

    void CPUSkin(Skeleton& skeleton, Pose& pose);
    void UpdateOpenGLBuffers();
    void Bind(int position, int normal, int texCoord, int weight, int influence);

    void Draw();
    void DrawInstanced(unsigned int numInstances);
    void UnBind(int position, int normal, int texCoord, int weight, int influence);
};
