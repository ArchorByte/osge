#include "vulkan.depth.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

/*
    Find and return the depth format.

    Tasks:
        1) Verify the parameters.
        2) Find a format that handles the depth format properties.

    Parameters:
        - physical_device / VkPhysicalDevice / Physical device used to run Vulkan.

    Returns:
        The depth format.
*/
VkFormat Vulkan::Depth::find_depth_format
(
    const VkPhysicalDevice &physical_device
)
{
    log("Looking for the depth format..");

    if (physical_device == VK_NULL_HANDLE)
        fatal_error_log("Failed to find such format! The physical device provided (" + force_string(physical_device) + ") is not valid!");

    const std::vector<VkFormat> candidate_formats =
    {
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT
    };

    for (const VkFormat &format : candidate_formats)
    {
        VkFormatProperties properties;
        vkGetPhysicalDeviceFormatProperties(physical_device, format, &properties);

        if ((properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) == VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        {
            log("Supported depth format found: " + std::to_string(format) + "!");
            return format;
        }
    }

    fatal_error_log("Failed to find the depth format!");
    return candidate_formats[0]; // Avoid compiler warnings.
}
