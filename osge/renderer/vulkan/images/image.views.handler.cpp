#include "vulkan.images.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>

/*
    Create a view for an image.
    Warning: There is no class that will automatically destroy this image view, you have to set one up yourself for memory safety reasons.

    Tasks:
        1) Verify the parameters.
        2) Create the view.

    Parameters:
        - aspect_flags   / VkImageAspectFlags / Flags of the image aspects.
        - format         / VkFormat           / Format of the image.
        - image          / VkImage            / Targeted image.
        - logical_device / VkDevice           / Logical device of the Vulkan instance.
        - mip_levels     / uint32_t           / Mip levels used for LOD.

    Returns:
        The created image view.
*/
VkImageView Vulkan::Images::create_image_view
(
    const VkImageAspectFlags &aspect_flags,
    const VkFormat &format,
    const VkImage &image,
    const VkDevice &logical_device,
    const uint32_t &mip_levels
)
{
    if (!format)
        fatal_error_log("Image view creation failed! The format provided (" + std::to_string(format) + ") is not valid!");

    if (image == VK_NULL_HANDLE)
        fatal_error_log("Image view creation failed! The image provided (" + force_string(image) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        fatal_error_log("Image view creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");

    if (mip_levels < 1)
        fatal_error_log("Image view creation failed! The mip levels count provided (" + std::to_string(mip_levels) + ") is not valid!");

    const VkImageViewCreateInfo info
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = format,
        .components =
        {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
        },
        .subresourceRange =
        {
            .aspectMask = aspect_flags,
            .baseMipLevel = 0,
            .levelCount = mip_levels,
            .baseArrayLayer = 0,
            .layerCount = 1
        }
    };

    VkImageView image_view = VK_NULL_HANDLE;
    const VkResult view_creation = vkCreateImageView(logical_device, &info, nullptr, &image_view);

    if (view_creation != VK_SUCCESS)
        fatal_error_log("Image view creation returned error code " + std::to_string(view_creation) + ".");

    return image_view;
}
