#include "vulkan.queues.hpp"
#include "osge/utils/utils.hpp"
#include <vulkan/vulkan.h>
#include <vector>

/*
    Get the graphics queue family index.

    Tasks:
        1) Verify the parameters.
        2) Check all queue families until we found the graphics one.

    Parameters:
        - queue_families / vector<VkQueueFamilyProperties> / List of all available queue families.

    Returns:
        The index of the graphics queue family.
*/
uint32_t Vulkan::Queues::get_graphics_queue_family_index
(
    const std::vector<VkQueueFamilyProperties> &queue_families
)
{
    Utils::Logs::log("Fetching the graphics queue family index..");

    if (queue_families.size() < 1)
        Utils::Logs::crash_error_log("Graphics queue family index query failed! No queue families provided!");

    uint32_t output = -1;
    int i = 0;

    for (const VkQueueFamilyProperties &queue : queue_families)
    {
        if (queue.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            output = i;
            break;
        }

        i++;
    }

    if (output == -1)
        Utils::Logs::crash_error_log("Graphics queue family index query failed! Failed to find any graphics queue!");

    Utils::Logs::log("Graphics queue family index found: " + std::to_string(output) + ".");
    return output;
}
