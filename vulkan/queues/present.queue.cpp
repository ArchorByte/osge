#include "present.queue.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <cstdint>
#include <vector>

// Return the index of the present family.
uint32_t get_present_family_index
(
    const std::vector<VkQueueFamilyProperties> &queue_families,
    const VkPhysicalDevice &physical_device,
    const VkSurfaceKHR &vulkan_surface
)
{
    log("Fetching the present queue family index..");

    if (queue_families.size() < 1)
    {
        fatal_error_log("Present family index query failed! No queue families were provided!");
    }

    if (!physical_device || physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Present family index query failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (!vulkan_surface || vulkan_surface == VK_NULL_HANDLE)
    {
        fatal_error_log("Present family index query failed! The Vulkan surface provided (" + force_string(vulkan_surface) + ") is not valid!");
    }

    uint32_t output = -1;
    int i = 0;

    // Analyze each queue family registered.
    for (const VkQueueFamilyProperties &queue : queue_families)
    {
        // Try to determine if the physical device handles the surfaces.
        VkBool32 support = VK_FALSE;
        VkResult query_result = vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, vulkan_surface, &support);

        if (query_result != VK_SUCCESS)
        {
            fatal_error_log("Physical device surface support query returned error code " + std::to_string(query_result) + ".");
        }

        if (support)
        {
            output = i;
            break;
        }

        i++;
    }

    if (output == -1)
    {
        fatal_error_log("Present family index query failed! Failed to find any present queue!");
    }

    log("Present queue family index found: " + std::to_string(output) + ".");
    return output;
}
