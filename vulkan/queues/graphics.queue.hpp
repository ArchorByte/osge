#include <vector>
#include <vulkan/vulkan.h>
#include <cstdint>

#ifndef VULKAN_GRAPHICS_QUEUE_HPP
#define VULKAN_GRAPHICS_QUEUE_HPP

uint32_t get_graphics_family_index
(
    const std::vector<VkQueueFamilyProperties> &queue_families
);

#endif
