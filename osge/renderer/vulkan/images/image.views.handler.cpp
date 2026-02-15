#include "vulkan.images.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

/*
    Create a view for an image.
    Warning: There is no class that will automatically destroy this image view, you have to set one up yourself for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Create image view.

    Parameters:
        - aspect_flags   / VkImageAspectFlags / Defines additional parameters for the image view to create.
        - format         / VkFormat           / Defines the format to use for the image view to create.
        - image          / VkImage            / Defines the image targeted by the view creation.
        - logical_device / VkDevice           / Logical device of this Vulkan instance.
        - mip_levels     / uint32_t           / Mip levels used for Level Of Details (LOD).

    Returns:
        The created image view.
*/
VkImageView Images::create_image_view
(
    const VkImageAspectFlags &aspect_flags,
    const VkFormat           &format,
    const VkImage            &image,
    const VkDevice           &logical_device,
    const uint32_t           &mip_levels
)
{
    if (!format)
        Utils::Logs::crash_error_log("Image view creation failed! The format provided (" + std::to_string(format) + ") is not valid!");

    if (image == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Image view creation failed! The image provided (" + Utils::Text::get_memory_address(image) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Image view creation failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    if (mip_levels < 1)
        Utils::Logs::crash_error_log("Image view creation failed! The mip levels count provided (" + std::to_string(mip_levels) + ") is not valid!");

    /*
        - sType              / Defines the type of the structure.
        - image              / Defines the format of the image targeted by the view creation.
        - viewType           / Defines the type of the image view to create.
        - format             / Defines the format to use for the image view to create.
        - components         / Defines the color structure of the image view.
            - r              / Red value.
            - g              / Green value.
            - b              / Blue value.
            - a              / Alpha/Transparency value.
        - subresourceRange   / Defines which mipmap levels and array layers are accessible to the image view.
            - aspectMask     / Defines which aspect of the original image will be included in the image view.
            - baseMipLevel   / Defines the first mipmap level accessible.
            - levelCount     / Defines the amount of mipmap levels that will be accessible.
            - baseArrayLayer / Defines the first array layer accessible.
            - layerCount     / Defines the amount of array layers that will be accessible.
    */
    const VkImageViewCreateInfo create_info
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
    const VkResult view_creation = vkCreateImageView(logical_device, &create_info, nullptr, &image_view);

    if (view_creation != VK_SUCCESS)
        Utils::Logs::crash_error_log("Image view creation returned error code " + std::to_string(view_creation) + ".");

    return image_view;
}
