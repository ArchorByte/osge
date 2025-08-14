#include "graphics_queue.hpp"
#include "../../logs/terminal.hpp"

#include <vulkan/vulkan.h>
#include <cstdint>
#include <vector>

// Return the index of the graphics family.
uint32_t get_graphics_family_index
(
    const std::vector<VkQueueFamilyProperties> &queue_families
)
{
    log("Fetching the graphics queue family index..");

    if (queue_families.size() < 1)
    {
        fatal_error_log("Graphics family index query failed! No queue families were provided!");
    }

    uint32_t output = -1;
    int i = 0;

    // Analyze each queue family registered.
    for (const VkQueueFamilyProperties &queue : queue_families)
    {
        // The graphics family is identified by the flag "VK_QUEUE_GRAPHICS_BIT".
        if (queue.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            output = i;
            break;
        }

        i++;
    }

    if (output == -1)
    {
        fatal_error_log("Graphics family index query failed! Failed to find any graphics queue!");
    }

    log("Graphics queue family index found: " + std::to_string(output) + ".");
    return output;
}
