#include <vulkan/vulkan.h>
#include <vector>

#ifndef VULKAN_COMMANDS_BUFFER_HPP
#define VULKAN_COMMANDS_BUFFER_HPP

std::vector<VkCommandBuffer> create_vulkan_command_buffers
(
    const VkDevice &logical_device,
    const VkCommandPool &command_pool,
    const uint32_t &images_count
);

#endif
