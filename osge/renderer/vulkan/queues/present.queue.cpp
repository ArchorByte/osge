#include "vulkan.queues.hpp"
#include "osge/utils/utils.hpp"
#include <libraries/vulkan/vulkan.h>
#include <vector>

/*
    Get the present queue family index.

    Tasks:
        1) Verify the parameters.
        2) Check all queue families until we found the present one.

    Parameters:
        - physical_device / VkPhysicalDevice                / Physical device used to run Vulkan.
        - queue_families  / vector<VkQueueFamilyProperties> / List of all available queue families.
        - vulkan_surface  / VkSurfaceKHR                    / Vulkan surface of the Vulkan instance and SDL3 window.

    Returns:
        The index of the present queue family.
*/
uint32_t Vulkan::Queues::get_present_queue_family_index
(
    const VkPhysicalDevice &physical_device,
    const std::vector<VkQueueFamilyProperties> &queue_families,
    const VkSurfaceKHR &vulkan_surface
)
{
    Utils::Logs::log("Fetching the present queue family index..");

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Present queue family index query failed! The physical device provided (" + Utils::Text::get_memory_address(physical_device) + ") is not valid!");

    if (queue_families.size() < 1)
        Utils::Logs::crash_error_log("Present queue family index query failed! No queue families provided!");

    if (vulkan_surface == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Present queue family index query failed! The Vulkan surface provided (" + Utils::Text::get_memory_address(vulkan_surface) + ") is not valid!");

    uint32_t output = -1;
    int i = 0;

    for (const VkQueueFamilyProperties &queue : queue_families)
    {
        VkBool32 supported = VK_FALSE;
        const VkResult query_result = vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, vulkan_surface, &supported);

        if (query_result != VK_SUCCESS)
            Utils::Logs::crash_error_log("Present queue family index query failed! Physical device surface support query returned error code " + std::to_string(query_result) + ".");

        if (supported)
        {
            output = i;
            break;
        }

        i++;
    }

    if (output == -1)
        Utils::Logs::crash_error_log("Present queue family index query failed! Failed to find any present queue!");

    Utils::Logs::log("Present queue family index found: " + std::to_string(output) + ".");
    return output;
}
