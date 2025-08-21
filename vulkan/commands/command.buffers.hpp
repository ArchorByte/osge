#include <vulkan/vulkan.h>
#include <vector>

#ifndef VULKAN_COMMAND_BUFFERS_HPP
#define VULKAN_COMMAND_BUFFERS_HPP

std::vector<VkCommandBuffer> create_vulkan_command_buffers
(
    const VkDevice &logical_device,
    const VkCommandPool &command_pool,
    const uint32_t &images_count
);

#endif
