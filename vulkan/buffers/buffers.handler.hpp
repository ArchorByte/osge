#include <vulkan/vulkan.h>

#ifndef VULKAN_BUFFERS_HANDLER_HPP
#define VULKAN_BUFFERS_HANDLER_HPP

void create_vulkan_buffer
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkDeviceSize &buffer_size,
    const VkBufferUsageFlags &usage_flags,
    const VkMemoryPropertyFlags &memory_properties_flags,
    VkBuffer &buffer,
    VkDeviceMemory &buffer_memory
);

void destroy_vulkan_buffer
(
    const VkDevice &logical_device,
    VkBuffer &buffer,
    VkDeviceMemory &buffer_memory
);

#endif
