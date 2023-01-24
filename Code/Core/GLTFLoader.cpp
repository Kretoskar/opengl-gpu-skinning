#include "GLTFLoader.h"

#include <cstring>
#include <iostream>

#include "Animation/Clip.h"
#include "Animation/Pose.h"
#include "Animation/Track.h"
#include "Animation/TransformTrack.h"
#include "Math/Mat4.h"
#include "Math/Transform.h"

namespace GLTFHelpers
{
    // Inside the GLTFHelpers namespace
    Transform GetLocalTransform(cgltf_node& n)
    {
        Transform result;

        if (n.has_matrix)
        {
            Mat4 mat(&n.matrix[0]);
            result = mat.ToTransform();
        }
        
        if (n.has_translation)
        {
            result.position = Vec3(n.translation[0], 
                 n.translation[1], n.translation[2]);
        }
        
        if (n.has_rotation)
        {
            result.rotation = Quat(n.rotation[0], 
              n.rotation[1], n.rotation[2], n.rotation[3]);
        }
        
        if (n.has_scale)
        {
            result.scale = Vec3(n.scale[0], n.scale[1], 
                                n.scale[2]);
        }
        
        return result;
    }

    int GetNodeIndex(cgltf_node* target, cgltf_node* allNodes, unsigned int numNodes)
    {
        if (target == nullptr)
        {
            return -1;
        }
        
        for (unsigned int i = 0; i < numNodes; ++i)
        {
            if (target == &allNodes[i])
            {
                return static_cast<int>(i);
            }
        }
        
        return -1;
    }

    void GetScalarValues( std::vector<float>& out, unsigned int compCount, const cgltf_accessor& inAccessor)
    {
        out.resize(inAccessor.count * compCount);

        for (cgltf_size i = 0; i < inAccessor.count; ++i)
        {
            cgltf_accessor_read_float(&inAccessor, i, 
                                      &out[i * compCount], 
                                      compCount);
        }
    }

    template<typename T, int N>
    void TrackFromChannel(Track<T, N>& result, const cgltf_animation_channel& channel)
    {
        cgltf_animation_sampler& sampler = *channel.sampler;

        Interpolation interpolation = Interpolation::Constant;

        if (sampler.interpolation == cgltf_interpolation_type_linear)
        {
            interpolation = Interpolation::Linear;
        }

        else if (sampler.interpolation == cgltf_interpolation_type_cubic_spline)
        {
            interpolation = Interpolation::Cubic;
        }

        bool isSamplerCubic = interpolation == Interpolation::Cubic;
        result.SetInterpolation(interpolation);

        std::vector<float> time; // times
        GetScalarValues(time, 1, *sampler.input);
        
        std::vector<float> val; // values
        GetScalarValues(val, N, *sampler.output);
        
        unsigned int numFrames = sampler.input->count; 
        unsigned int compCount = val.size() / time.size();
        result.Resize(numFrames);

        for (unsigned int i = 0; i < numFrames; ++i)
        {
            int baseIndex = i * compCount;
            Frame<N>& frame = result[i];
            int offset = 0;
            frame.mTime = time[i];

            for (int comp = 0; comp < N; ++comp)
            {
                frame.mIn[comp] = isSamplerCubic ? 
                      val[baseIndex + offset++] : 0.0f;
            }

            for (int comp = 0; comp < N; ++comp)
            {
                frame.mValue[comp] = val[baseIndex + 
                                     offset++];
            }

            for (int comp = 0; comp < N; ++comp)
            {
                frame.mOut[comp] = isSamplerCubic ? 
                      val[baseIndex + offset++] : 0.0f;
            }
        }
    }
}

cgltf_data* LoadGLTFFile(const char* path)
{
    constexpr cgltf_options options = {};
    cgltf_data* data = nullptr;
    cgltf_result result = cgltf_parse_file(&options, path, &data);

    if(result != cgltf_result_success)
    {
        std::cout << "Could not load: " << path << "\n";
        return nullptr;
    }

    result = cgltf_load_buffers(&options,  data, path);
    if(result != cgltf_result_success)
    {
        cgltf_free(data);
        std::cout << "Could not load: " << path << "\n";
        return nullptr;
    }

    result = cgltf_validate(data);
    if(result != cgltf_result_success)
    {
        cgltf_free(data);
        std::cout << "Could not load: " << path << "\n";
        return nullptr;
    }

    return data;
}

void FreeGLTFFile(cgltf_data* data)
{
    if(data == nullptr)
    {
        std::cout << "WARNING: Can't free null data\n";
    }
    else
    {
        cgltf_free(data);
    }
}

Pose LoadRestPose(cgltf_data* data)
{
    unsigned int boneCount = data->nodes_count;
    Pose result(boneCount);

    for (unsigned int i = 0; i < boneCount; ++i)
    {
        cgltf_node* node = &(data->nodes[i]);
        Transform transform = GLTFHelpers::GetLocalTransform(data->nodes[i]);
        result.SetLocalTransform(i, transform);

        int parent = GLTFHelpers::GetNodeIndex(
                     node->parent, data->nodes, 
                     boneCount);

        result.SetParent(i, parent);
    }

    return result;
}

std::vector<std::string> LoadJointNames(cgltf_data* data)
{
    unsigned int boneCount = static_cast<unsigned int>(data->nodes_count);

    std::vector<std::string> result(boneCount, "Not Set");

    for (unsigned int i = 0; i < boneCount; ++i)
    {
        cgltf_node* node = &(data->nodes[i]);
        result[i] = node->name == nullptr ? "EMPTY NODE" : node->name;
    }
    
    return result;
}

std::vector<Clip> LoadAnimationClips(cgltf_data* data)
{
    const unsigned int numClips = data->animations_count;
    const unsigned int numNodes = data->nodes_count;
    
    std::vector<Clip> result;
    result.resize(numClips);

    for (unsigned int i = 0; i < numClips; ++i)
    {
        result[i].SetName(data->animations[i].name);

        const unsigned int numChannels = data->animations[i].channels_count;

        for (unsigned int j = 0; j < numChannels; ++j)
        {
            cgltf_animation_channel& channel = 
                      data->animations[i].channels[j];

            cgltf_node* target = channel.target_node;

            const int nodeId = GLTFHelpers::GetNodeIndex(target, data->nodes, numNodes);

            if (channel.target_path == cgltf_animation_path_type_translation)
            {
                VectorTrack& track = result[i][nodeId].GetPositionTrack();
                GLTFHelpers::TrackFromChannel<Vec3, 3> (track, channel);
            }

            else if (channel.target_path == cgltf_animation_path_type_scale)
            {
                VectorTrack& track = result[i][nodeId].GetScaleTrack();
                GLTFHelpers::TrackFromChannel<Vec3, 3>
                            (track, channel);
            }

            else if (channel.target_path == cgltf_animation_path_type_rotation)
            {
                QuaternionTrack& track = result[i][nodeId].GetRotationTrack();
                GLTFHelpers::TrackFromChannel<Quat, 4>
                             (track, channel);
            }
        }
        result[i].RecalculateDuration();
    }
    return result;
} 