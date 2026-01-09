#include "vulkan.colors.hpp"

#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>

/*
    Create a color attachment description for a graphics pipeline.

    Tasks:
        1) Create the attachment.

    Parameters:
        - samples_count  / VkSampleCountFlagBits / Amount of samples to use for the MSAA.
        - surface_format / VkFormat              / Surface format of the Vulkan instance.
*/
VkAttachmentDescription Vulkan::Colors::create_color_attachment
(
    const VkSampleCountFlagBits &samples_count,
    const VkFormat &surface_format
)
{
    log("Creating a color attachment description..");

    const VkAttachmentDescription attachment
    {
        .format = surface_format,
        .samples = samples_count,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,   // Clear this attachment when the render pass starts.
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE, // Store the render result.
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    log("Color attachment description created successfully!");
    return attachment;
}
