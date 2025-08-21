#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

#ifndef VULKAN_PRESENT_QUEUE_HPP
#define VULKAN_PRESENT_QUEUE_HPP

uint32_t get_present_family_index
(
    const std::vector<VkQueueFamilyProperties> &queue_families,
    const VkPhysicalDevice &physical_device,
    const VkSurfaceKHR &vulkan_surface
);

#endif
