#include <vulkan/vulkan.h>

#ifndef VULKAN_COMMAND_WRAPPER_HPP
#define VULKAN_COMMAND_WRAPPER_HPP

VkCommandBuffer begin_one_time_vulkan_command_buffer
(
    const VkDevice &logical_device,
    const VkCommandPool &command_pool
);

void end_command_buffer
(
    const VkDevice &logical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    VkCommandBuffer &command_buffer
);

#endif
