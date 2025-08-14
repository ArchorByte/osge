#include <vulkan/vulkan.h>

#ifndef VULKAN_PIPELINES_COLORS_HPP
#define VULKAN_PIPELINES_COLORS_HPP

VkAttachmentDescription create_vulkan_color_attachment
(
    const VkFormat &surface_format
);

#endif
