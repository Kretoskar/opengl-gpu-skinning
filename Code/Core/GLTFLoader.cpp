#include "GLTFLoader.h"

#include <cstring>
#include <iostream>

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
