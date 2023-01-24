#pragma once

#ifndef _H_GLTFLOADER_
#define _H_GLTFLOADER_

#include <string>

#include "cgltf.h"
#include "Animation/Mesh.h"
#include "Animation/Pose.h"
#include "Animation/Skeleton.h"

class Clip;

cgltf_data* LoadGLTFFile(const char* path);
void FreeGLTFFile(cgltf_data* data);
Pose LoadRestPose(cgltf_data* data);

std::vector<std::string> LoadJointNames(cgltf_data* data);

std::vector<Clip> LoadAnimationClips(cgltf_data* data);

Pose LoadBindPose(cgltf_data* data);

Skeleton LoadSkeleton(cgltf_data* data);

std::vector<Mesh> LoadMeshes(cgltf_data* data);

#endif