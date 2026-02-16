#include "vulkan.depth.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

/*
    Create a depth attachment.

    Tasks:
        1) Verify function parameters.
        2) Create the depth attachment.

    Parameters:
        - physical_device / VkPhysicalDevice      / Physical device used to run this Vulkan instance.
        - samples_count   / VkSampleCountFlagBits / Amount of samples to render at the same time for multisampling.

    Returns:
        The created depth attachment.
*/
VkAttachmentDescription Depth::create_depth_attachment
(
    const VkPhysicalDevice      &physical_device,
    const VkSampleCountFlagBits &samples_count
)
{
    Utils::Logs::log("Creating depth attachment..", false);

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical device invalid.");

    if (samples_count < 1)
        Utils::Logs::crash_log("Failed! Samples count invalid -> " + std::to_string(samples_count) + ".");

    /*
        - format         / Defines the format of the image view for the attachment.
        - samples        / Defines the amount of samples to make for an image. Enables multisampling.
        - loadOp         / Defines how the content of the attachment will be used by the subpass. Here, we forces the render area to be cleared.
        - storeOp        / Defines what happens to the content of the attachment once the subpass ended. Here, we just ignore the content.
        - stencilLoadOp  / Defines how the content of the stencil components will be used by the subpass. Here, we just ignore the previous data and overwrite it.
        - stencilStoreOp / Defines what happens to the content of the stencil components once the subpass ended. Here, we just ignore the content.
        - initialLayout  / Defines the initial layout of the attachment.
        - finalLayout    / Defines the layout to transition to once the render pass ended.
    */
    const VkAttachmentDescription depth_attachment
    {
        .format = Depth::find_depth_format(physical_device),
        .samples = samples_count,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };

    Utils::Logs::log("Done!", true);
    return depth_attachment;
}
