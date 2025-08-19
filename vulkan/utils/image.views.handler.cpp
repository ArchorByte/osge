#include "image.views.handler.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>

// Create a view for an image.
VkImageView create_image_view
(
    const VkDevice &logical_device,
    const VkImage &image,
    const VkFormat &format
)
{
    // Create info for the image view
    VkImageViewCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.image = image;                                           // Pass the swap chain image.
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;                        // Use 2D mode.
    info.format = format;                                         // Pass the image format.
    info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;            // Default red identity.
    info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;            // Default green identity.
    info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;            // Default blue identity.
    info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;            // Default alpha identity.
    info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // This image view will be for the color aspect.
    info.subresourceRange.baseMipLevel = 0;                       // Set the starting mipmap level.
    info.subresourceRange.levelCount = 1;                         // Amount of mipmap levels that we will use.
    info.subresourceRange.baseArrayLayer = 0;                     // Set the starting layer.
    info.subresourceRange.layerCount = 1;                         // Amount of layers that we will use.

    VkImageView image_view = VK_NULL_HANDLE;
    VkResult view_creation = vkCreateImageView(logical_device, &info, nullptr, &image_view); // Try to create the image view.

    if (view_creation != VK_SUCCESS)
    {
        fatal_error_log("Image view creation returned error code " + std::to_string(view_creation) + ".");
    }

    if (!image_view || image_view == VK_NULL_HANDLE)
    {
        fatal_error_log("Image view creation output \"" + force_string(image_view) + "\" is not valid!");
    }

    return image_view;
}
