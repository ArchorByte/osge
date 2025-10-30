#include "depth.formats.hpp"

#include "../helpers/supported.format.hpp"

#include <vulkan/vulkan.h>

// Find and return the depth format.
VkFormat find_depth_format
(
    const VkPhysicalDevice &physical_device
)
{
    const VkFormat format = find_supported_format
    (
        physical_device,
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );

    return format;
}

// Check if a depth format has a stencil component.
bool has_stencil_component
(
    const VkFormat &format
)
{
    const bool verification = format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    return verification;
}
