#include <vulkan/vulkan.h>
#include <cstdint>

#ifndef VULKAN_UNIFORM_UPDATE_HPP
#define VULKAN_UNIFORM_UPDATE_HPP

void update_uniform_buffer
(
    const uint32_t &frame,
    const VkExtent2D extent,
    const void* buffer_data
);

#endif
