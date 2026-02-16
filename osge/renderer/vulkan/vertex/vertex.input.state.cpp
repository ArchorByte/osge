#include "vulkan.vertex.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

/*
    Create a vertex input state.

    Tasks:
        1) Create vertex input state.
        2) Return it.

    Parameters:
        No parameters.

    Returns:
        The created vertex input state.
*/
VkPipelineVertexInputStateCreateInfo Vertex::create_vertex_input_state()
{
    Utils::Logs::log("Creating vertex input state.. ", false);

    const static auto binding_description = Vertex::VertexObj::get_binding_description();
    const static auto attribute_descriptions = Vertex::VertexObj::get_attribute_descriptions();

    /*
        - sType                           / Defines the type of the structure.
        - vertexBindingDescriptionCount   / Defines the amount of vertex binding descriptions to pass.
        - pVertexBindingDescriptions      / Passes the vertex binding descriptions.
        - vertexAttributeDescriptionCount / Defines the amount of vertex attribute descriptions to pass.
        - pVertexAttributeDescriptions    / Passes the vertex attribute descriptions.
    */
    const VkPipelineVertexInputStateCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 1,
        .pVertexBindingDescriptions = &binding_description,
        .vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size()),
        .pVertexAttributeDescriptions = attribute_descriptions.data()
    };

    Utils::Logs::log("Done!", true);
    return create_info;
}
