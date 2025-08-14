#include <vulkan/vulkan.h>
#include <vector>

#ifndef VULKAN_CORE_EXTENSIONS_HPP
#define VULKAN_CORE_EXTENSIONS_HPP

void check_vulkan_extensions_support
(
    const VkPhysicalDevice &physical_device,
    const std::vector<const char *> &required_extensions
);

#endif
