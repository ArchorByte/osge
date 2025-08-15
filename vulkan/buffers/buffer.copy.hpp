#include "../textures/texture.images.loader.hpp"

#include <vulkan/vulkan.h>

#ifndef VULKAN_BUFFERS_COPY_HPP
#define VULKAN_BUFFERS_COPY_HPP

void copy_vulkan_buffer_data
(
    const VkDevice &logical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkBuffer &source_buffer,
    const VkBuffer &destination_buffer,
    const VkDeviceSize &buffer_size
);

void copy_vulkan_buffer_to_texture_image
(
    const VkDevice &logical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkBuffer &buffer,
    const VkImage &image,
    const TextureImageInfo &image_info
);

#endif
