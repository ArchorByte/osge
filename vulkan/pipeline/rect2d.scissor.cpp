#include "rect2d.scissor.hpp"

#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>

// Create a scissor for a graphics pipeline.
VkRect2D create_vulkan_scissor
(
    const VkExtent2D &extent
)
{
    log("Creating a scissor..");

    VkRect2D scissor
    {
        .offset = { 0, 0 }, // Select the starting position.
        .extent = extent
    };

    log("Scissor created successfully!");
    return scissor;
}
