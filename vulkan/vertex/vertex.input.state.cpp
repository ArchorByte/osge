#include "vertex.input.state.hpp"

#include "vertex.handler.hpp"
#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>

// Create a vertex input state for a graphics pipeline.
VkPipelineVertexInputStateCreateInfo create_vulkan_vertex_input_state()
{
    log("Creating a vertex input state..");

    static auto binding_description = Vertex::get_binding_description();       // Retrieve the binding description from the Vertex shader data.
    static auto attribute_descriptions = Vertex::get_attribute_descriptions(); // Retrieve the attribute descriptions from the Vertex shader data.

    // Create info for the Vertex input state.
    VkPipelineVertexInputStateCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    info.vertexBindingDescriptionCount = 1;                                                      // Amount of binding descriptions to pass.
    info.pVertexBindingDescriptions = &binding_description;                                      // Pass the binding description.
    info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size()); // Amount of attribute descriptions to pass.
    info.pVertexAttributeDescriptions = attribute_descriptions.data();                           // Pass the attribute descriptions.

    log("Vertex input state created successfully!");
    return info;
}
