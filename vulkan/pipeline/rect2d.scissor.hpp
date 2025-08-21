#include <vulkan/vulkan.h>

#ifndef VULKAN_PIPELINE_SCISSOR_HPP
#define VULKAN_PIPELINE_SCISSOR_HPP

VkRect2D create_vulkan_scissor
(
    const VkExtent2D &extent
);

#endif
