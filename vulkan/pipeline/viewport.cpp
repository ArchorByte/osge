#include "viewport.hpp"

#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>

// Create a viewport for a graphics pipeline.
VkViewport create_vulkan_viewport
(
    const VkExtent2D &extent
)
{
    log("Creating a viewport..");

    // Create the viewport for rendering.
    VkViewport viewport
    {
        .x = 0.0f, // Starting x position of the viewport.
        .y = 0.0f, // Starting y position.
        .width = (float) extent.width,
        .height = (float) extent.height,
        .minDepth = 0.0f, // Anything drawn before will be clipped at position 0.
        .maxDepth = 1.0f  // Anything drawn further will be clipped at position 1.
    };

    log("Viewport created successfully!");
    return viewport;
}

// Create a viewport state for a graphics pipeline.
VkPipelineViewportStateCreateInfo create_viewport_state()
{
    log("Creating a viewport state..");

    // Create info for the viewport state.
    VkPipelineViewportStateCreateInfo viewport_state_create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .pViewports = nullptr, // Don't directly pass the viewport here in order to use dynamic states.
        .scissorCount = 1,
        .pScissors = nullptr,
    };

    log("Viewport state created successfully!");
    return viewport_state_create_info;
}
