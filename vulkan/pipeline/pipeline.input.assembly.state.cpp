#include "assembly_state.hpp"

#include "../vertex/vertex.hpp"
#include "../../logs/terminal.hpp"

#include <vulkan/vulkan.h>
#include <cstdint>

// Create an assembly input state for a graphics pipeline.
VkPipelineInputAssemblyStateCreateInfo create_vulkan_assembly_input_state()
{
    log("Creating an assembly input state..");

    // Info for the assembly state.
    VkPipelineInputAssemblyStateCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; // Interpret a sequence of 3 numbers in the vertices as a triangle.
    info.primitiveRestartEnable = VK_FALSE;              // Disable primitive restarts.

    log("Assembly input state created successfully!");
    return info;
}
