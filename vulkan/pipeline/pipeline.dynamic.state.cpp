#include "dynamic_state.hpp"
#include "../../logs/terminal.hpp"

#include <vulkan/vulkan.h>
#include <vector>

// Create some dynamic states for a graphics pipeline.
VkPipelineDynamicStateCreateInfo create_vulkan_dynamic_states()
{
    log("Creating a pipeline dynamic state..");

    // Dynamic states that we want to use.
    static std::vector<VkDynamicState> states =
    {
        VK_DYNAMIC_STATE_VIEWPORT, // Dynamic state for the viewport.
        VK_DYNAMIC_STATE_SCISSOR   // Dynamic state for the scissor.
    };

    // Create info for the dynamic state.
    VkPipelineDynamicStateCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    info.dynamicStateCount = static_cast<uint32_t>(states.size()); // Amount of dynamic states to use.
    info.pDynamicStates = states.data();                           // Pass the list of dynamic states to use.

    log("Pipeline dynamic state created successfully!");
    return info;
}
