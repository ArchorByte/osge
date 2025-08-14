#include <vulkan/vulkan.h>

#ifndef VULKAN_PIPELINES_VIEWPORT_HPP
#define VULKAN_PIPELINES_VIEWPORT_HPP

VkViewport create_vulkan_viewport
(
    const VkExtent2D &extent
);

VkPipelineViewportStateCreateInfo create_viewport_state();

#endif
