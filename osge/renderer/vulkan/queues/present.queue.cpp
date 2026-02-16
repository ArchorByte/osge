#include "vulkan.queues.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <vector>

/*
    Get the present queue family index.

    Tasks:
        1) Verify function parameters.
        2) Check all queue families until we find it.

    Parameters:
        - physical_device / VkPhysicalDevice                / Physical device used to run this Vulkan instance.
        - queue_families  / vector<VkQueueFamilyProperties> / List of all available queue families.
        - vulkan_surface  / VkSurfaceKHR                    / Link between this Vulkan instance and the SDL3 game window.

    Returns:
        The index of the present queue family.
*/
uint32_t Queues::get_present_queue_family_index
(
    const VkPhysicalDevice                     &physical_device,
    const std::vector<VkQueueFamilyProperties> &queue_families,
    const VkSurfaceKHR                         &vulkan_surface
)
{
    Utils::Logs::log("Fetching present queue family index.. ", false);

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical device invalid.");

    if (queue_families.size() < 1)
        Utils::Logs::crash_log("Failed! No queue families provided.");

    if (vulkan_surface == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Vulkan surface invalid.");

    uint32_t output = -1;
    int i = 0;

    for (const VkQueueFamilyProperties &queue : queue_families)
    {
        VkBool32 supported = VK_FALSE;
        const VkResult query_result = vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, vulkan_surface, &supported);

        if (query_result != VK_SUCCESS)
            continue;

        if (supported)
        {
            output = i;
            break;
        }

        i++;
    }

    if (output == -1)
        Utils::Logs::crash_log("Failed! No present queue found.");

    Utils::Logs::log("Done! Index -> " + std::to_string(output) + ".", true);
    return output;
}
