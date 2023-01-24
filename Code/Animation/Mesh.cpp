#include "Mesh.h"

#include "Rendering/Draw.h"

Mesh::Mesh()
{
    mPosAttrib = new Attribute<Vec3>();
    mNormAttrib = new Attribute<Vec3>();
    mUvAttrib = new Attribute<Vec2>();
    mWeightAttrib = new Attribute<Vec4>();
    mInfluenceAttrib = new Attribute<IVec4>();
    mIndexBuffer = new IndexBuffer();
}

Mesh::Mesh(const Mesh& other)
{
    mPosAttrib = new Attribute<Vec3>();
    mNormAttrib = new Attribute<Vec3>();
    mUvAttrib = new Attribute<Vec2>();
    mWeightAttrib = new Attribute<Vec4>();
    mInfluenceAttrib = new Attribute<IVec4>();
    mIndexBuffer = new IndexBuffer();
    *this = other;
}

Mesh& Mesh::operator=(const Mesh& other)
{
    if (this == &other)
    {
        return *this;
    }
    
    mPosition = other.mPosition;
    mNormal = other.mNormal;
    mTexCoord = other.mTexCoord;
    mWeights = other.mWeights;
    mInfluences = other.mInfluences;
    mIndices = other.mIndices;

    UpdateOpenGLBuffers();

    return *this;
}

Mesh::~Mesh()
{
    delete mPosAttrib;
    delete mNormAttrib;
    delete mUvAttrib;
    delete mWeightAttrib;
    delete mInfluenceAttrib;
    delete mIndexBuffer;
}

std::vector<Vec3>& Mesh::GetPosition()
{
    return mPosition;
}

std::vector<Vec3>& Mesh::GetNormal()
{
    return mNormal;
}

std::vector<Vec2>& Mesh::GetTexCoord()
{
    return mTexCoord;
}

std::vector<Vec4>& Mesh::GetWeights()
{
    return mWeights;
}

std::vector<IVec4>& Mesh::GetInfluences()
{
    return mInfluences;
}

std::vector<unsigned int>& Mesh::GetIndices()
{
    return mIndices;
}

void Mesh::UpdateOpenGLBuffers()
{
    if (!mPosition.empty())
    {
        mPosAttrib->Set(mPosition);
    }
    
    if (!mNormal.empty())
    {
        mNormAttrib->Set(mNormal);
    }
    
    if (!mTexCoord.empty())
    {
        mUvAttrib->Set(mTexCoord);
    }
    
    if (!mWeights.empty())
    {
        mWeightAttrib->Set(mWeights);
    }
    
    if (!mInfluences.empty())
    {
        mInfluenceAttrib->Set(mInfluences);
    }
    
    if (!mIndices.empty())
    {
        mIndexBuffer->Set(mIndices);
    }
}

void Mesh::Bind(int position, int normal, int texCoord, int weight, int influcence)
{
    if (position >= 0)
    {
        mPosAttrib->BindTo(position);
    }

    if (normal >= 0)
    {
        mNormAttrib->BindTo(normal);
    }
    
    if (texCoord >= 0)
    {
        mUvAttrib->BindTo(texCoord);
    }
    
    if (weight >= 0)
    {
        mWeightAttrib->BindTo(weight);
    }
    
    if (influcence >= 0)
    {
        mInfluenceAttrib->BindTo(influcence);
    }
}

void Mesh::Draw()
{
    if (mIndices.size() > 0)
    {
        ::Draw(*mIndexBuffer, DrawMode::Triangles);
    }
    else
    {
        ::Draw(mPosition.size(), DrawMode::Triangles);
    }
}

void Mesh::DrawInstanced(unsigned int numInstances)
{
    if (mIndices.size() > 0)
    {
        ::DrawInstanced(*mIndexBuffer, 
          DrawMode::Triangles, numInstances);
    }
    
    else
    {
        ::DrawInstanced(mPosition.size(), 
          DrawMode::Triangles, numInstances);
    }
}

void Mesh::UnBind(int position, int normal, int texCoord, int weight, int influence)
{
    if (position >= 0)
    {
        mPosAttrib->UnBindFrom(position);
    }
    
    if (normal >= 0)
    {
        mNormAttrib->UnBindFrom(normal);
    }
    
    if (texCoord >= 0)
    {
        mUvAttrib->UnBindFrom(texCoord);
    }
    
    if (weight >= 0)
    {
        mWeightAttrib->UnBindFrom(weight);
    }

    if (influence >= 0)
    {
        mInfluenceAttrib->UnBindFrom(influence);
    }
}

void Mesh::CPUSkin(Skeleton& skeleton, Pose& pose)
{
    unsigned int numVerts = static_cast<unsigned int>(mPosition.size());
    if (numVerts == 0)
    {
        return;
    }
    
    mSkinnedPosition.resize(numVerts);
    mSkinnedNormal.resize(numVerts);
    pose.GetMatrixPalette(mPosePalette);

    std::vector<Mat4> invPosePalette = skeleton.GetInvBindPose();
    
    for (unsigned int i = 0; i < numVerts; ++i)
    {
        IVec4& j = mInfluences[i];
        Vec4& w = mWeights[i];

        Mat4 m0 = (mPosePalette[j.x] * invPosePalette[j.x]) * w.x;
        Mat4 m1 = (mPosePalette[j.y] * invPosePalette[j.y]) * w.y;
        Mat4 m2 = (mPosePalette[j.z] * invPosePalette[j.z]) * w.z;
        Mat4 m3 = (mPosePalette[j.w] * invPosePalette[j.w]) * w.w;
        Mat4 skin = m0 + m1 + m2 + m3;
        
        mSkinnedPosition[i] = Mat4::TransformPoint(skin,mPosition[i]);
        mSkinnedNormal[i] = Mat4::TransformVector(skin, mNormal[i]);
    }
    
    mPosAttrib->Set(mSkinnedPosition);
    mNormAttrib->Set(mSkinnedNormal);
}