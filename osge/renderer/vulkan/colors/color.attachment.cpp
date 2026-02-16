#include "vulkan.colors.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

/*
    Create a color attachment description for a graphics pipeline.

    Tasks:
        1) Create the attachment.

    Parameters:
        - samples_count  / VkSampleCountFlagBits / Amount of samples to use for multisampling.
        - surface_format / VkFormat              / Defines the format of the image view that will be used for the attachment.

    Returns:
        The created color attachment.
*/
VkAttachmentDescription Colors::create_color_attachment
(
    const VkSampleCountFlagBits &samples_count,
    const VkFormat              &surface_format
)
{
    Utils::Logs::log("Creating a color attachment description.. ", false);

    /*
        - format         / Defines the format of the image view that will be used for the attachment.
        - samples        / Defines how many samples we are going to use for the image. Enables multisampling.
        - loadOp         / Defines how the content of the attachment will be used by the subpass. Here, we forces the render area to be cleared.
        - storeOp        / Defines what happens to the content of the attachment once the subpass ended. Here, we just ignore the content.
        - stencilLoadOp  / Defines how the content of the stencil components will be used by the subpass. Here, we just ignore the previous data and overwrite it.
        - stencilStoreOp / Defines what happens to the content of the stencil components once the subpass ended. Here, we just ignore the content.
        - initialLayout  / Defines the initial layout of the attachment.
        - finalLayout    / Defines the layout to transition to once the render pass ended.
    */
    const VkAttachmentDescription attachment
    {
        .format = surface_format,
        .samples = samples_count,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    Utils::Logs::log("Done!", true);
    return attachment;
}
