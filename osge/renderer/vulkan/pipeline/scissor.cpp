#include "vulkan.pipeline.hpp"

#include "../../../utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"

/*
    Create a scissor for a pipeline.

    Tasks:
        1) Make scissor.
        2) Return it.

    Parameters:
        - extent / VkExtent2D / Resolution of the swap chain.

    Returns:
        The created scissor.
*/
VkRect2D Pipeline::create_scissor
(
    const VkExtent2D &extent
)
{
    Utils::Logs::log("Creating scissor.. ", false);

    /*
        - offset / Defines the starting position of the scissor.
        - extent / Defines the area affected.
    */
    const VkRect2D scissor
    {
        .offset = { 0, 0 },
        .extent = extent
    };

    Utils::Logs::log("Done!", true);
    return scissor;
}
