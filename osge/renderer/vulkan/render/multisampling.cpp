#include "vulkan.render.hpp"

#include "../../../utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"

/*
    Get and return the highest amount of samples this machine supports.

    Tasks:
        1) Verify function parameters.
        2) Get physical device properties.
        3) Get sample count data from properties.
        4) Determine the highest sample count supported.

    Parameters:
        - physical_device / VkPhysicalDevice / Physical device used to run this Vulkan instance.

    Returns:
        The highest sample count available.
*/
VkSampleCountFlagBits Render::get_max_sample_count
(
    const VkSampleCountFlagBits &custom_count,
    const VkPhysicalDevice      &physical_device
)
{
    Utils::Logs::log("Detecting highest sample count available.. ", false);

    if (physical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Physical device invalid, defaulted to 1.", true);
        return VK_SAMPLE_COUNT_1_BIT;
    }

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(physical_device, &properties);

    const VkSampleCountFlags counts = properties.limits.framebufferColorSampleCounts & properties.limits.framebufferDepthSampleCounts;
    VkSampleCountFlagBits output = VK_SAMPLE_COUNT_1_BIT;

    if (counts & VK_SAMPLE_COUNT_64_BIT)
        output = VK_SAMPLE_COUNT_64_BIT;
    else if (counts & VK_SAMPLE_COUNT_32_BIT)
        output = VK_SAMPLE_COUNT_32_BIT;
    else if (counts & VK_SAMPLE_COUNT_16_BIT)
        output = VK_SAMPLE_COUNT_16_BIT;
    else if (counts & VK_SAMPLE_COUNT_8_BIT)
        output = VK_SAMPLE_COUNT_8_BIT;
    else if (counts & VK_SAMPLE_COUNT_4_BIT)
        output = VK_SAMPLE_COUNT_4_BIT;
    else if (counts & VK_SAMPLE_COUNT_2_BIT)
        output = VK_SAMPLE_COUNT_2_BIT;

    if (custom_count & output)
        output = custom_count;

    Utils::Logs::log("Done! Sample count -> " + std::to_string(VK_SAMPLE_COUNT_1_BIT) + ".", true);
    return output;
}



/*
    Convert an integer into a sample count flags bit.
    Note: Valid values are 1, 2, 4, 8, 16, 32 and 64. Otherwise, we default to VK_SAMPLE_COUNT_1_BIT.

    Tasks:
        1) Switch-case statement that looks for the correct flag.
        2) Return VK_SAMPLE_COUNT_1_BIT for 1 and any invalid values.

    Parameters:
        - samples_count / int / Amount of samples to convert.

    Returns:
        A sample count flags bits corresponding to the input.
*/
VkSampleCountFlagBits Render::to_sample_count_flags_bit
(
    const int &sample_count
)
{
    switch (sample_count)
    {
        case 64:
            return VK_SAMPLE_COUNT_64_BIT;
        case 32:
            return VK_SAMPLE_COUNT_32_BIT;
        case 16:
            return VK_SAMPLE_COUNT_16_BIT;
        case 8:
            return VK_SAMPLE_COUNT_8_BIT;
        case 4:
            return VK_SAMPLE_COUNT_4_BIT;
        case 2:
            return VK_SAMPLE_COUNT_2_BIT;
        default:
            return VK_SAMPLE_COUNT_1_BIT;
    }
}
