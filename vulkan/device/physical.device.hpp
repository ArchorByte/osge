#include <vulkan/vulkan.h>
#include <string>

#ifndef VULKAN_PHYSICAL_DEVICE_HPP
#define VULKAN_PHYSICAL_DEVICE_HPP

bool is_valid_physical_device
(
    const VkPhysicalDevice &physical_device
);

std::string get_physical_device_name
(
    const VkPhysicalDevice &physical_device
);

VkPhysicalDevice select_physical_device
(
    const VkInstance &vulkan_instance
);

#endif
