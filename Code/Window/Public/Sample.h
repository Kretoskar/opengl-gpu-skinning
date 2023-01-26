#pragma once

#include "Window/Public/Application.h"
#include <vector>
#include "Animation/Public/Pose.h"
#include "Animation/Public/Clip.h"
#include "Animation/Public/Skeleton.h"
#include "Rendering/Public/Mesh.h"
#include "OpenGL/Public/Texture.h"
#include "OpenGL/Public/Shader.h"
#include <vector>

struct AnimationInstance
{
    Pose mAnimatedPose;
    std::vector<Mat4> mPosePalette;
    unsigned int mClip;
    float mPlayback;
    Transform mModel;

    AnimationInstance() : mClip(0), mPlayback(0.0f)
    {
    }
};

class Sample : public Application
{
protected:
    Texture* mDiffuseTexture;
    Shader* mStaticShader;
    Shader* mSkinnedShader;
    std::vector<Mesh> mCPUMeshes;
    std::vector<Mesh> mGPUMeshes;
    Skeleton mSkeleton;
    std::vector<Clip> mClips;

    AnimationInstance mGPUAnimInfo;
    AnimationInstance mCPUAnimInfo;
public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(float inAspectRatio) override;
    void Shutdown() override;
};
