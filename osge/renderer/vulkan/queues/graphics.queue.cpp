#include "vulkan.queues.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <vector>

/*
    Get the graphics queue family index.

    Tasks:
        1) Verify function parameters.
        2) Check all queue families until we find it.

    Parameters:
        - queue_families / vector<VkQueueFamilyProperties> / List of all available queue families.

    Returns:
        The index of the graphics queue family.
*/
uint32_t Queues::get_graphics_queue_family_index
(
    const std::vector<VkQueueFamilyProperties> &queue_families
)
{
    Utils::Logs::log("Fetching graphics queue family index.. ", false);

    if (queue_families.size() < 1)
        Utils::Logs::crash_log("Failed! No queue families provided.");

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
        Utils::Logs::crash_log("Failed! No graphics queue found.");

    Utils::Logs::log("Done! Index -> " + std::to_string(output) + ".", true);
    return output;
}
