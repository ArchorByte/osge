#include <vulkan/vulkan.h>

#ifndef VULKAN_UTILS_IMAGE_VIEWS_HPP
#define VULKAN_UTILS_IMAGE_VIEWS_HPP

VkImageView create_image_view
(
    const VkDevice &logical_device,
    const VkImage &image,
    const VkFormat &format,
    const VkImageAspectFlags &aspect_flags,
    const uint32_t &mip_levels
);

#endif
