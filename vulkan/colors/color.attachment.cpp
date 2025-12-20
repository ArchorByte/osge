#include "color.attachment.hpp"

#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>

// Create a color attachment description for a graphics pipeline.
VkAttachmentDescription create_vulkan_color_attachment
(
    const VkFormat &surface_format,
    const VkSampleCountFlagBits &samples_count
)
{
    log("Creating a color attachment description..");

    // Description of out color attachment.
    VkAttachmentDescription description
    {
        .format = surface_format,                               // Pass the surface format.
        .samples = samples_count,                               // MSAA.
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,                  // Clear this attachment when the render pass start.
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,                // Store the render result.
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,       // Ignore the stencil buffer. No need to load anything.
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,     // Same. No need to store anything.
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,             // Ignore the initial layout.
        .finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL // Use a valid layout for the frame presents.
    };

    log("Color attachment description created successfully!");
    return description;
}
