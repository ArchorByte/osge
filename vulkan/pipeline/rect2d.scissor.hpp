#include <vulkan/vulkan.h>

#ifndef VULKAN_PIPELINES_SCISSOR_HPP
#define VULKAN_PIPELINES_SCISSOR_HPP

VkRect2D create_vulkan_scissor
(
    const VkExtent2D &extent
);

#endif
