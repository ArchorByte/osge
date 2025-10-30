#include <vulkan/vulkan.h>

#ifndef VULKAN_DEPTH_FORMATS_HPP
#define VULKAN_DEPTH_FORMATS_HPP

VkFormat find_depth_format
(
    const VkPhysicalDevice &physical_device
);

bool has_stencil_component
(
    const VkFormat &format
);

#endif
