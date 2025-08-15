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

    // Create the viewport for rendering surface selection.
    VkViewport viewport{};
    viewport.x = 0.0f;                       // Start x position of the viewport.
    viewport.y = 0.0f;                       // Start y position of the viewport.
    viewport.width = (float) extent.width;   // Width of the viewport.
    viewport.height = (float) extent.height; // Height of the viewport.
    viewport.minDepth = 0.0f;                // Minimum depth range. Anything drawn before will be clipped at position 0.
    viewport.maxDepth = 1.0f;                // Maximum depth range. Anything drawn further will be clipped at position 1.

    log("Viewport created successfully!");
    return viewport;
}

// Create a viewport state for a graphics pipeline.
VkPipelineViewportStateCreateInfo create_viewport_state()
{
    log("Creating a viewport state..");

    // Create info for the viewport state.
    VkPipelineViewportStateCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    info.viewportCount = 1;    // Amount of viewport used.
    info.pViewports = nullptr; // We don't direcly pass the viewport here because we use a dynamic state.
    info.scissorCount = 1;     // Amount of scissor used.
    info.pScissors = nullptr;  // Same here.

    log("Viewport state created successfully!");
    return info;
}
