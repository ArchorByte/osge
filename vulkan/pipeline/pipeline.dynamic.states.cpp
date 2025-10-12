#include "pipeline.dynamic.states.hpp"

#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>
#include <vector>

// Create some dynamic states for a graphics pipeline.
VkPipelineDynamicStateCreateInfo create_vulkan_dynamic_states()
{
    log("Creating a pipeline dynamic state..");

    // Dynamic states that we want to enable.
    const static std::vector<VkDynamicState> dynamic_states =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    // Create info for the dynamic state.
    VkPipelineDynamicStateCreateInfo dynamic_state_create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = static_cast<uint32_t>(dynamic_states.size()), // Amount of dynamic states to use.
        .pDynamicStates = dynamic_states.data()
    };

    log("Pipeline dynamic state created successfully!");
    return dynamic_state_create_info;
}
