#include <vulkan/vulkan.h>

#ifndef VULKAN_RENDER_MULTISAMPLING_HPP
#define VULKAN_RENDER_MULTISAMPLING_HPP

VkSampleCountFlagBits get_max_sample_count
(
    const VkPhysicalDevice &physical_device
);

#endif
