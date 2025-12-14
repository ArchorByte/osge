#include <vulkan/vulkan.h>
#include <utility>

#ifndef VULKAN_HELPERS_IMAGES_HANDLER_HPP
#define VULKAN_HELPERS_IMAGES_HANDLER_HPP

std::pair<VkImage, VkDeviceMemory> create_image
(
    const VkPhysicalDevice &physical_device,
    const VkDevice &logical_device,
    const int &width,
    const int &height,
    const uint32_t &mip_levels,
    const VkFormat &format,
    const VkImageTiling &tiling,
    const VkImageUsageFlags &usage_flags
);

#endif
