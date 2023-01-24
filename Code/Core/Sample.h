#pragma once

#include "Application.h"
#include "Animation/Pose.h"
#include "Animation/Clip.h"
#include "Animation/Skeleton.h"
#include "Animation/Mesh.h"
#include "Rendering/Texture.h"
#include "Rendering/Shader.h"
#include "Animation/TransformTrack.h"

#include <vector>

struct AnimationInstance
{
    Pose mAnimatedPose;
    std::vector <Mat4> mPosePalette;
    unsigned int mClip;
    float mPlayback;
    Transform mModel;

    inline AnimationInstance() : mClip(0), mPlayback(0.0f) { }
};

class Sample : public Application {
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
    void Initialize();
    void Update(float deltaTime);
    void Render(float inAspectRatio);
    void Shutdown();
};