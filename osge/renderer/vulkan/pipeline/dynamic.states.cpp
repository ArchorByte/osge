#include "vulkan.pipeline.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <vector>

/*
    Create dynamic states for a pipeline.

    Tasks:
        1) Defines the list of dynamic states to enable.
        2) Make the create info.

    Parameters:
        No parameters.

    Returns:
        The created pipeline dynamic state create info.
*/
VkPipelineDynamicStateCreateInfo Pipeline::create_dynamic_states()
{
    Utils::Logs::log("Creating pipeline dynamic state.. ", false);

    const static std::vector<VkDynamicState> dynamic_states =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    /*
        - sType             / Defines the type of the structure.
        - dynamicStateCount / Defines the amount of dynamic states to enable.
        - pDynamicStates    / Passes the dynamic states to enable.
    */
    const VkPipelineDynamicStateCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = static_cast<uint32_t>(dynamic_states.size()),
        .pDynamicStates = dynamic_states.data()
    };

    Utils::Logs::log("Done!", true);
    return create_info;
}
