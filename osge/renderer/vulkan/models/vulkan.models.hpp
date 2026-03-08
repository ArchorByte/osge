#include <filesystem>
#include <vector>

#include "../vertex/vulkan.vertex.hpp"

#ifndef VULKAN_MODELS_HPP
#define VULKAN_MODELS_HPP

namespace Models
{
    /////////////////////////////
    ///// models.loader.cpp /////
    /////////////////////////////

    void load_3d_models
    (
        std::vector<uint32_t>          &indices,
        std::vector<Vertex::VertexObj> &vertices
    );

    //////////////////////////////////
    ///// models.obj.handler.cpp /////
    //////////////////////////////////

    void load_obj_model
    (
        const std::filesystem::path    &file_path,
        std::vector<uint32_t>          &indices,
        std::vector<Vertex::VertexObj> &vertices
    );
}

#endif
