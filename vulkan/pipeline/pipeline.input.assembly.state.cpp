#include "pipeline.input.assembly.state.hpp"

#include "../vertex/vertex.handler.hpp"
#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>
#include <cstdint>

// Create an assembly input state for a graphics pipeline.
VkPipelineInputAssemblyStateCreateInfo create_vulkan_assembly_input_state()
{
    log("Creating an assembly input state..");

    const VkPipelineInputAssemblyStateCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE // Disable primitive restarts.
    };

    log("Assembly input state created successfully!");
    return create_info;
}
