#include <vulkan/vulkan.h>

#ifndef VULKAN_TEXTURE_IMAGE_TRANSITIONS_HPP
#define VULKAN_TEXTURE_IMAGE_TRANSITIONS_HPP

void transition_image_layout
(
    const VkDevice &logical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkImage &image,
    const VkFormat &format,
    const VkImageLayout &old_layout,
    const VkImageLayout &new_layout
);

#endif
