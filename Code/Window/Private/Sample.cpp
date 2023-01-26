#pragma warning(disable : 26451)
#define _CRT_SECURE_NO_WARNINGS
#include "Window/Public/Sample.h"
#include "GLTF/Public/GLTFLoader.h"
#include "OpenGL/Public/Uniform.h"
#include "Window/Public/glad.h"

void Sample::Initialize()
{
    cgltf_data* gltf = LoadGLTFFile("Assets/Woman.gltf");
    mCPUMeshes = LoadMeshes(gltf);
    mSkeleton = LoadSkeleton(gltf);
    mClips = LoadAnimationClips(gltf);
    FreeGLTFFile(gltf);

    mGPUMeshes = mCPUMeshes;
    for (unsigned int i = 0, size = static_cast<unsigned>(mGPUMeshes.size()); i < size; ++i)
    {
        mGPUMeshes[i].UpdateOpenGLBuffers();
    }

    mStaticShader = new Shader("Shaders/static.vert", "Shaders/lit.frag");
    mSkinnedShader = new Shader("Shaders/skinned.vert", "Shaders/lit.frag");
    mDiffuseTexture = new Texture("Assets/Woman.png");

    mGPUAnimInfo.mAnimatedPose = mSkeleton.GetRestPose();
    mGPUAnimInfo.mPosePalette.resize(mSkeleton.GetRestPose().Size());
    mCPUAnimInfo.mAnimatedPose = mSkeleton.GetRestPose();
    mCPUAnimInfo.mPosePalette.resize(mSkeleton.GetRestPose().Size());

    mGPUAnimInfo.mModel.position = Vec3(-2, 0, 0);
    mCPUAnimInfo.mModel.position = Vec3(2, 0, 0);

    unsigned int numUIClips = static_cast<unsigned>(mClips.size());
    for (unsigned int i = 0; i < numUIClips; ++i)
    {
        if (mClips[i].GetName() == "Walking")
        {
            mCPUAnimInfo.mClip = i;
        }
        else if (mClips[i].GetName() == "Running")
        {
            mGPUAnimInfo.mClip = i;
        }
    }
}

void Sample::Update(float deltaTime)
{
    mCPUAnimInfo.mPlayback = mClips[mCPUAnimInfo.mClip].Sample(mCPUAnimInfo.mAnimatedPose,
                                                               mCPUAnimInfo.mPlayback + deltaTime);
    mGPUAnimInfo.mPlayback = mClips[mGPUAnimInfo.mClip].Sample(mGPUAnimInfo.mAnimatedPose,
                                                               mGPUAnimInfo.mPlayback + deltaTime);

    for (unsigned int i = 0, size = static_cast<unsigned>(mCPUMeshes.size()); i < size; ++i)
    {
        mCPUMeshes[i].CPUSkin(mSkeleton, mCPUAnimInfo.mAnimatedPose);
    }

    mGPUAnimInfo.mAnimatedPose.GetMatrixPalette(mGPUAnimInfo.mPosePalette);
}

void Sample::Render(float inAspectRatio)
{
    Mat4 projection = Mat4::Perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
    Mat4 view = Mat4::LookAt(Vec3(-10, 5, 7), Vec3(-2, 2.5, 0), Vec3(0, 1, 0));
    Mat4 model;

    /*
    // CPU Skinned Mesh
    model = (mCPUAnimInfo.mModel).ToMat4();
    mStaticShader->Bind();
    Uniform<Mat4>::Set(mStaticShader->GetUniform("model"), model);
    Uniform<Mat4>::Set(mStaticShader->GetUniform("view"), view);
    Uniform<Mat4>::Set(mStaticShader->GetUniform("projection"), projection);
    Uniform<Vec3>::Set(mStaticShader->GetUniform("light"), Vec3(1, 1, 1));

    mDiffuseTexture->Set(mStaticShader->GetUniform("tex0"), 0);
    for (unsigned int i = 0, size = static_cast<unsigned>(mCPUMeshes.size()); i < size; ++i)
    {
        mCPUMeshes[i].Bind(mStaticShader->GetAttribute("position"), mStaticShader->GetAttribute("normal"),
                           mStaticShader->GetAttribute("texCoord"), -1, -1);
        mCPUMeshes[i].Draw();
        mCPUMeshes[i].UnBind(mStaticShader->GetAttribute("position"), mStaticShader->GetAttribute("normal"),
                             mStaticShader->GetAttribute("texCoord"), -1, -1);
    }
    mDiffuseTexture->UnSet(0);
    mStaticShader->UnBind();
    */
    
    // GPU Skinned Mesh
    model = (mGPUAnimInfo.mModel).ToMat4();
    mSkinnedShader->Bind();
    Uniform<Mat4>::Set(mSkinnedShader->GetUniform("model"), model);
    Uniform<Mat4>::Set(mSkinnedShader->GetUniform("view"), view);
    Uniform<Mat4>::Set(mSkinnedShader->GetUniform("projection"), projection);
    Uniform<Vec3>::Set(mSkinnedShader->GetUniform("light"), Vec3(-5, 5, 1));

    Uniform<Mat4>::Set(mSkinnedShader->GetUniform("pose"), mGPUAnimInfo.mPosePalette);
    Uniform<Mat4>::Set(mSkinnedShader->GetUniform("invBindPose"), mSkeleton.GetInvBindPose());

    mDiffuseTexture->Set(mSkinnedShader->GetUniform("tex0"), 0);
    for (unsigned int i = 0, size = static_cast<unsigned>(mGPUMeshes.size()); i < size; ++i)
    {
        mGPUMeshes[i].Bind(mSkinnedShader->GetAttribute("position"), mSkinnedShader->GetAttribute("normal"),
                           mSkinnedShader->GetAttribute("texCoord"), mSkinnedShader->GetAttribute("weights"),
                           mSkinnedShader->GetAttribute("joints"));
        mGPUMeshes[i].Draw();
        mGPUMeshes[i].UnBind(mSkinnedShader->GetAttribute("position"), mSkinnedShader->GetAttribute("normal"),
                             mSkinnedShader->GetAttribute("texCoord"), mSkinnedShader->GetAttribute("weights"),
                             mSkinnedShader->GetAttribute("joints"));
    }
    mDiffuseTexture->UnSet(0);
    mSkinnedShader->UnBind();
}

void Sample::Shutdown()
{
    delete mStaticShader;
    delete mDiffuseTexture;
    delete mSkinnedShader;
    mClips.clear();
    mCPUMeshes.clear();
    mGPUMeshes.clear();
}
