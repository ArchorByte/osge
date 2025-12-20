#include "depth.attachments.hpp"

#include "depth.formats.hpp"
#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>

// Create a depth attachment.
VkAttachmentDescription create_depth_attachment
(
    const VkPhysicalDevice &physical_device,
    const VkSampleCountFlagBits &samples_count
)
{
    log("Creating a depth attachment..");

    const VkAttachmentDescription depth_attachment
    {
        .format = find_depth_format(physical_device),
        .samples = samples_count,                    // MSAA.
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,       // Clear depth buffer.
        .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE, // Don't store depth buffer data after render pass.
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,   // Ignore stencil buffer loading.
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE, // Ignore stencil buffer storing.
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };

    log("Depth attachment created successfully!");
    return depth_attachment;
}

// Create a depth attachment reference.
VkAttachmentReference create_depth_attachment_reference()
{
    log("Creating a depth attachment reference..");

    VkAttachmentReference reference
    {
        .attachment = 1,
        .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };

    log("Depth attachment reference created successfully!");
    return reference;
}
