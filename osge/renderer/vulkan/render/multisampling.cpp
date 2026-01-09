#include "multisampling.hpp"

#include "../device/physical.device.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>

// Return the amount of samples we can make at the same time.
VkSampleCountFlagBits get_max_sample_count
(
    const VkPhysicalDevice &physical_device
)
{
    if (physical_device == VK_NULL_HANDLE)
    {
        error_log("Failed to determine the max sample count! The physical device provided (" + force_string(physical_device) + ") is not valid!");
        return VK_SAMPLE_COUNT_1_BIT;
    }

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(physical_device, &properties);
    VkSampleCountFlags counts = properties.limits.framebufferColorSampleCounts & properties.limits.framebufferDepthSampleCounts;

    if (counts & VK_SAMPLE_COUNT_64_BIT)
        return VK_SAMPLE_COUNT_64_BIT;
    else if (counts & VK_SAMPLE_COUNT_32_BIT)
        return VK_SAMPLE_COUNT_32_BIT;
    else if (counts & VK_SAMPLE_COUNT_16_BIT)
        return VK_SAMPLE_COUNT_16_BIT;
    else if (counts & VK_SAMPLE_COUNT_8_BIT)
        return VK_SAMPLE_COUNT_8_BIT;
    else if (counts & VK_SAMPLE_COUNT_4_BIT)
        return VK_SAMPLE_COUNT_4_BIT;
    else if (counts & VK_SAMPLE_COUNT_2_BIT)
        return VK_SAMPLE_COUNT_2_BIT;
    else
        return VK_SAMPLE_COUNT_1_BIT;
}
