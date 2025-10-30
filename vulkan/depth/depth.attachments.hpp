#include <vulkan/vulkan.h>

#ifndef VULKAN_DEPTH_ATTACHMENTS_HPP
#define VULKAN_DEPTH_ATTACHMENTS_HPP

VkAttachmentDescription create_depth_attachment
(
    const VkPhysicalDevice &physical_device
);

VkAttachmentReference create_depth_attachment_reference();

#endif
