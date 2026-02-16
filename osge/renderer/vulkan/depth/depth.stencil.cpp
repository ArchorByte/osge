#include "vulkan.depth.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

/*
    Create a depth stencil for a graphics pipeline.

    Tasks:
        1) Create the depth stencil.

    Parameters:
        No parameters.

    Returns:
        The created depth stencil.
*/
VkPipelineDepthStencilStateCreateInfo Depth::create_depth_stencil()
{
    Utils::Logs::log("Creating depth stencil.. ", false);

    /*
        - sType                 / Defines the type of the structure.
        - depthTestEnable       / Defines whether depth testing is enabled or not.
        - depthWriteEnable      / Defines if we enable depth writing or not.
        - depthCompareOp        / Defines which operation shall be used for depth comparison operations.
        - depthBoundsTestEnable / Defines whether depth bounds testing is enabled or not.
        - stencilTestEnable     / Defines whether stencil testing is enabled or not.
    */
    const VkPipelineDepthStencilStateCreateInfo depth_stencil
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depthTestEnable = VK_TRUE,
        .depthWriteEnable = VK_TRUE,
        .depthCompareOp = VK_COMPARE_OP_LESS,
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable = VK_FALSE
    };

    Utils::Logs::log("Done!", true);
    return depth_stencil;
}
