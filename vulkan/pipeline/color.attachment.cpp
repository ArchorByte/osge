#include "color.attachment.hpp"

#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>

// Create a color attachment description for a graphics pipeline.
VkAttachmentDescription create_vulkan_color_attachment
(
    const VkFormat &surface_format
)
{
    log("Creating a color attachment description..");

    // Description of out color attachment.
    VkAttachmentDescription description {};
    description.format = surface_format;                           // Pass the surface format.
    description.samples = VK_SAMPLE_COUNT_1_BIT;                   // No multisampling.
    description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;              // Clear this attachment when the render pass start.
    description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;            // Store the render result.
    description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;   // Ignore the stencil buffer. No need to load anything.
    description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // Same. No need to store anything.
    description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;         // Ignore the initial layout.
    description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;     // Use a valid layout for the frame presents.

    log("Color attachment description created successfully!");
    return description;
}
