#include "vulkan.pipeline.hpp"

#include "../../../utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"

/*
    Create a rasterization state for a pipeline.

    Tasks:
        1) Make create info.
        2) Return it.

    Parameters:
        No parameters.

    Returns:
        The created pipeline rasterization state create info.
*/
VkPipelineRasterizationStateCreateInfo Pipeline::create_rasterization_state()
{
    Utils::Logs::log("Creating rasterization state.. ", false);

    /*
        - sType                   / Defines the type of the structure.
        - depthClampEnable        / Defines whether we clamp the fragment's depth values or not.
        - rasterizerDiscardEnable / Defines whether primitives are discarded immediately before the rasterization stage or not.
        - polygonMode             / Defines triangle rendering mode.
        - cullMode                / Defines triangle facing direction used for primitive culling.
        - frontFace               / Defines the front-facing triangle orientation to be used for culling.
        - depthBiasEnable         / Defines whether to bias fragment depth values or not.
    */
    const VkPipelineRasterizationStateCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
        .depthBiasEnable = VK_FALSE
    };

    Utils::Logs::log("Done!", true);
    return create_info;
}
