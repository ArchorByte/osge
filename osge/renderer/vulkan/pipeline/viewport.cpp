#include "vulkan.pipeline.hpp"

#include "../../../utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"

/*
    Create a viewport for a pipeline.

    Tasks:
        1) Make viewport.
        2) Return it.

    Parameters:
        - extent / VkExtent2D / Resolution of the swap chain.

    Returns:
        The created viewport.
*/
VkViewport Pipeline::create_viewport
(
    const VkExtent2D &extent
)
{
    Utils::Logs::log("Creating viewport.. ", false);

    /*
        - x        / Defines x starting position.
        - y        / Defines y starting position.
        - width    / Defines the width of the viewport.
        - height   / Defines the height of the viewport.
        - minDepth / Defines minimum value for depth.
        - maxDepth / Defines maximum value for depth.
    */
    const VkViewport viewport
    {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float) extent.width,
        .height = (float) extent.height,
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };

    Utils::Logs::log("Done!", true);
    return viewport;
}



/*
    Create a viewport state for a pipeline.

    Tasks:
        1) Make create info.
        2) Return it.

    Parameters:
        No parameters.

    Returns:
        The created pipeline viewport state create info.
*/
VkPipelineViewportStateCreateInfo Pipeline::create_viewport_state()
{
    Utils::Logs::log("Creating viewport state.. ", false);

    /*
        - sType         / Defines the type of the structure.
        - viewportCount / Amount of viewports to pass.
        - pViewports    / Passes the viewports. Here, we set to null in order to use dynamic states.
        - scissorCount  / Amount of scissors to pass.
        - pScissors     / Passes the scissors. Here, we set to null in order to use dynamic states.
    */
    const VkPipelineViewportStateCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .pViewports = nullptr,
        .scissorCount = 1,
        .pScissors = nullptr,
    };

    Utils::Logs::log("Done!", true);
    return create_info;
}
