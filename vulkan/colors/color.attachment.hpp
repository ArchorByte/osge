#include <vulkan/vulkan.h>

#ifndef VULKAN_PIPELINE_COLOR_ATTACHMENT_HPP
#define VULKAN_PIPELINE_COLOR_ATTACHMENT_HPP

VkAttachmentDescription create_vulkan_color_attachment
(
    const VkFormat &surface_format,
    const VkSampleCountFlagBits &samples_count
);

#endif
