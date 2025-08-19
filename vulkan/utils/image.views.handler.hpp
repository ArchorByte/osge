#include <vulkan/vulkan.h>

#ifndef VULKAN_UTILS_IMAGES_VIEWS_HPP
#define VULKAN_UTILS_IMAGES_VIEWS_HPP

VkImageView create_image_view
(
    const VkDevice &logical_device,
    const VkImage &image,
    const VkFormat &format
);

#endif
