#include <vulkan/vulkan.h>
#include <vector>

#ifndef VULKAN_HELPERS_SUPPORTED_FORMAT_HPP
#define VULKAN_HELPERS_SUPPORTED_FORMAT_HPP

VkFormat find_supported_format
(
    const VkPhysicalDevice &physical_device,
    const std::vector<VkFormat> &formats,
    const VkImageTiling &image_tiling,
    const VkFormatFeatureFlags &feature_flags
);

#endif
