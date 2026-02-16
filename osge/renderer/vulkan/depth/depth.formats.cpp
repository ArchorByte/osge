#include "vulkan.depth.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <string>
#include <vector>

/*
    Find and return the depth format.

    Tasks:
        1) Verify function parameters.
        2) Find a format that handles depth format properties.

    Parameters:
        - physical_device / VkPhysicalDevice / Physical device used to run this Vulkan instance.

    Returns:
        The depth format.
*/
VkFormat Depth::find_depth_format
(
    const VkPhysicalDevice &physical_device
)
{
    Utils::Logs::log("Looking for depth format.. ", false);

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical device invalid.");

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
            Utils::Logs::log("Done! Format -> " + std::to_string(format) + ".", true);
            return format;
        }
    }

    Utils::Logs::crash_log("Failed! No depth format found.");
    return candidate_formats[0]; // Avoid compiler warnings.
}
