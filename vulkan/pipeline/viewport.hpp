#include <vulkan/vulkan.h>

#ifndef VULKAN_PIPELINE_VIEWPORT_HPP
#define VULKAN_PIPELINE_VIEWPORT_HPP

VkViewport create_vulkan_viewport
(
    const VkExtent2D &extent
);

VkPipelineViewportStateCreateInfo create_viewport_state();

#endif
