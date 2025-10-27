#include "supported.format.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>

// Find a format supported by the physical device that matches the given criteria.
VkFormat find_supported_format
(
    const VkPhysicalDevice &physical_device,
    const std::vector<VkFormat> &formats,
    const VkImageTiling &image_tiling,
    const VkFormatFeatureFlags &feature_flags
)
{
    log("Looking for a format which handles " + std::to_string(image_tiling) + " and " + std::to_string(feature_flags) + "..");

    if (physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Failed to find such format! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    for (const VkFormat &format : formats)
    {
        VkFormatProperties properties;
        vkGetPhysicalDeviceFormatProperties(physical_device, format, &properties);

        if (image_tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & feature_flags) == feature_flags)
        {
            log("Supported format found: " + std::to_string(format) + "!");
            return format;
        }
        else if (image_tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & feature_flags) == feature_flags)
        {
            log("Supported format found: " + std::to_string(format) + "!");
            return format;
        }
    }

    fatal_error_log("Failed to find such format! Verify your requirements and/or hardware!");
    return formats[0]; // Pointless but prevent compiler warnings.
}
