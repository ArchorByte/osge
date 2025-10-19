#include "vertex.input.state.hpp"

#include "vertex.handler.hpp"
#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>

// Create a vertex input state for a graphics pipeline.
VkPipelineVertexInputStateCreateInfo create_vulkan_vertex_input_state()
{
    log("Creating a vertex input state..");

    const static auto binding_description = Vertex::get_binding_description();
    const static auto attribute_descriptions = Vertex::get_attribute_descriptions();

    const VkPipelineVertexInputStateCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 1,
        .pVertexBindingDescriptions = &binding_description,
        .vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size()), // Amount of attribute descriptions to pass.
        .pVertexAttributeDescriptions = attribute_descriptions.data()
    };

    log("Vertex input state created successfully!");
    return create_info;
}
