#include "vulkan.depth.hpp"
#include "osge/utils/utils.hpp"
#include <libraries/vulkan/vulkan.h>

/*
    Create a depth attachment.

    Tasks:
        1) Verify the parameters.
        2) Create the depth attachment.

    Parameters:
        - physical_device / VkPhysicalDevice      / Physical device used to run Vulkan.
        - samples_count   / VkSampleCountFlagBits / Amount of samples to render at the same time for multisampling.

    Returns:
        The created depth attachment.
*/
VkAttachmentDescription create_depth_attachment
(
    const VkPhysicalDevice &physical_device,
    const VkSampleCountFlagBits &samples_count
)
{
    Utils::Logs::log("Creating a depth attachment..");

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Depth attachment creation failed! The physical device provided (" + Utils::Text::get_memory_address(physical_device) + ") is not valid!");

    const VkAttachmentDescription depth_attachment
    {
        .format = Vulkan::Depth::find_depth_format(physical_device),
        .samples = samples_count,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };

    Utils::Logs::log("Depth attachment created successfully!");
    return depth_attachment;
}
