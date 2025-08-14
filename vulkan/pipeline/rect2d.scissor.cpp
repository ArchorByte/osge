#include "scissor.hpp"
#include "../../logs/terminal.hpp"

#include <vulkan/vulkan.h>

// Create a scissor for a graphics pipeline.
VkRect2D create_vulkan_scissor
(
    const VkExtent2D &extent
)
{
    log("Creating a scissor..");

    VkRect2D scissor {};
    scissor.offset = { 0, 0 }; // Select the start position of the scissor.
    scissor.extent = extent;   // Pass the swap chain extent.

    log("Scissor created successfully!");
    return scissor;
}
