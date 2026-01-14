#include "vulkan.pipeline.hpp"
#include "osge/utils/utils.hpp"
#include <libraries/vulkan/vulkan.h>
#include <vector>

// Create some dynamic states for a graphics pipeline.
VkPipelineDynamicStateCreateInfo create_vulkan_dynamic_states()
{
    Utils::Logs::log("Creating a pipeline dynamic state..");

    // Dynamic states to enable.
    const static std::vector<VkDynamicState> dynamic_states =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    const VkPipelineDynamicStateCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = static_cast<uint32_t>(dynamic_states.size()),
        .pDynamicStates = dynamic_states.data()
    };

    Utils::Logs::log("Pipeline dynamic state created successfully!");
    return create_info;
}
