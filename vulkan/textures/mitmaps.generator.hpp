#include <vulkan/vulkan.h>

#ifndef VULKAN_TEXTURES_MITMAPS_GENERATOR_HPP
#define VULKAN_TEXTURES_MITMAPS_GENERATOR_HPP

void generate_mipmaps
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkImage &image,
    const VkFormat &image_format,
    const int &width,
    const int &height,
    const uint32_t &mip_levels
);

#endif
