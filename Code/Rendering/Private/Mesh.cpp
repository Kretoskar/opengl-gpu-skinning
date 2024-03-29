#include "Rendering/Public/Mesh.h"
#include "OpenGL/Public/Draw.h"
#include "Math/Public/Transform.h"

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
    if (mPosition.size() > 0)
    {
        mPosAttrib->Set(mPosition);
    }
    if (mNormal.size() > 0)
    {
        mNormAttrib->Set(mNormal);
    }
    if (mTexCoord.size() > 0)
    {
        mUvAttrib->Set(mTexCoord);
    }
    if (mWeights.size() > 0)
    {
        mWeightAttrib->Set(mWeights);
    }
    if (mInfluences.size() > 0)
    {
        mInfluenceAttrib->Set(mInfluences);
    }
    if (mIndices.size() > 0)
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
        ::DrawInstanced(*mIndexBuffer, DrawMode::Triangles, numInstances);
    }
    else
    {
        ::DrawInstanced(mPosition.size(), DrawMode::Triangles, numInstances);
    }
}

void Mesh::UnBind(int position, int normal, int texCoord, int weight, int influcence)
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
    if (influcence >= 0)
    {
        mInfluenceAttrib->UnBindFrom(influcence);
    }
}

#if 1
void Mesh::CPUSkin(Skeleton& skeleton, Pose& pose)
{
    unsigned int numVerts = static_cast<unsigned>(mPosition.size());
    if (numVerts == 0) { return; }

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

        mSkinnedPosition[i] = Mat4::TransformPoint(skin, mPosition[i]);
        mSkinnedNormal[i] = Mat4::TransformVector(skin, mNormal[i]);
    }

    mPosAttrib->Set(mSkinnedPosition);
    mNormAttrib->Set(mSkinnedNormal);
}
#else
void Mesh::CPUSkin(Skeleton& skeleton, Pose& pose) {
	unsigned int numVerts = (unsigned int)mPosition.size();
	if (numVerts == 0) { return; }

	mSkinnedPosition.resize(numVerts);
	mSkinnedNormal.resize(numVerts);
	Pose& bindPose = skeleton.GetBindPose();

	for (unsigned int i = 0; i < numVerts; ++i) {
		ivec4& joint = mInfluences[i];
		vec4& weight = mWeights[i];

		Transform skin0 = combine(pose[joint.x], inverse(bindPose[joint.x]));
		vec3 p0 = transformPoint(skin0, mPosition[i]);
		vec3 n0 = transformVector(skin0, mNormal[i]);

		Transform skin1 = combine(pose[joint.y], inverse(bindPose[joint.y]));
		vec3 p1 = transformPoint(skin1, mPosition[i]);
		vec3 n1 = transformVector(skin1, mNormal[i]);

		Transform skin2 = combine(pose[joint.z], inverse(bindPose[joint.z]));
		vec3 p2 = transformPoint(skin2, mPosition[i]);
		vec3 n2 = transformVector(skin2, mNormal[i]);

		Transform skin3 = combine(pose[joint.w], inverse(bindPose[joint.w]));
		vec3 p3 = transformPoint(skin3, mPosition[i]);
		vec3 n3 = transformVector(skin3, mNormal[i]);
		mSkinnedPosition[i] = p0 * weight.x + p1 * weight.y + p2 * weight.z + p3 * weight.w;
		mSkinnedNormal[i] = n0 * weight.x + n1 * weight.y + n2 * weight.z + n3 * weight.w;
	}

	mPosAttrib->Set(mSkinnedPosition);
	mNormAttrib->Set(mSkinnedNormal);
}
#endif
