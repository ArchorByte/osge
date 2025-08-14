#include <cstdint>
#include <vulkan/vulkan.h>

#ifndef VULKAN_BUFFERS_MEMORY_HPP
#define VULKAN_BUFFERS_MEMORY_HPP

uint32_t find_memory_type
(
    const uint32_t &type_filter,
    const VkPhysicalDeviceMemoryProperties &memory_properties,
    const VkMemoryPropertyFlags &properties
);

VkDeviceMemory allocate_vulkan_buffer_memory
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkBuffer &buffer
);

#endif
