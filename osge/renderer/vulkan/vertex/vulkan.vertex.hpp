#define GLM_ENABLE_EXPERIMENTAL

#include "libraries/glm/glm.hpp"
#include "libraries/glm/gtx/hash.hpp"
#include "libraries/vulkan/vulkan.h"

#include <array>
#include <vector>

#ifndef VULKAN_VERTEX_HANDLER_HPP
#define VULKAN_VERTEX_HANDLER_HPP

namespace Vertex
{
    ///////////////////////////////////////////////////
    //////////////////// Structure ////////////////////
    ///////////////////////////////////////////////////

    struct VertexObj
    {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 texture_coordinates;

        //////////////////////////////
        ///// vertex.handler.cpp /////
        //////////////////////////////
        static VkVertexInputBindingDescription get_binding_description();
        static std::array<VkVertexInputAttributeDescription, 3> get_attribute_descriptions();

        bool operator==(const VertexObj& other) const
        {
            return position == other.position && color == other.color && texture_coordinates == other.texture_coordinates;
        }
    };

    //////////////////////////////////////////////////
    //////////////////// Function ////////////////////
    //////////////////////////////////////////////////

    //////////////////////////////////
    ///// vertex.input.state.cpp /////
    //////////////////////////////////

    VkPipelineVertexInputStateCreateInfo create_vertex_input_state();
}

namespace std
{
    template<> struct hash<Vertex::VertexObj>
    {
        size_t operator()(Vertex::VertexObj const& vertex) const
        {
            return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texture_coordinates) << 1);
        }
    };
}

#endif
