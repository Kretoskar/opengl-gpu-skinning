#pragma once

#ifndef _H_GLTFLOADER_
#define _H_GLTFLOADER_

#include <string>

#include "cgltf.h"
#include "Animation/Pose.h"

class Clip;

cgltf_data* LoadGLTFFile(const char* path);
void FreeGLTFFile(cgltf_data* data);
Pose LoadRestPose(cgltf_data* data);

std::vector<std::string> LoadJointNames(cgltf_data* data);

std::vector<Clip> LoadAnimationClips(cgltf_data* data);

#endif