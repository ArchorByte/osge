#include "vulkan.queues.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <set>
#include <vector>

/*
    Get all available queue families for a physical device.

    Tasks:
        1) Verify function parameters.
        2) Retrieve all queues.

    Parameters:
        - physical_device / VkPhysicalDevice / Physical device used to run this Vulkan instance.

    Returns:
        A vector list containing all queue families.
*/
std::vector<VkQueueFamilyProperties> Queues::get_queue_families
(
    const VkPhysicalDevice &physical_device
)
{
    Utils::Logs::log("Fetching the queue families list.. ", false);

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical device invalid.");

    uint32_t families_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &families_count, nullptr);

    if (families_count == 0)
        Utils::Logs::crash_log("Failed! No queue families found on this physical device.");

    std::vector<VkQueueFamilyProperties> queue_families(families_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &families_count, queue_families.data());

    Utils::Logs::log("Done! " + std::to_string(families_count) + " queue families found.", true);
    return queue_families;
}



/*
    Make all queues create info.

    Tasks:
        1) Verify function parameters.
        2) Make a create info for each queue family.

    Parameters:
        - physical_device / VkPhysicalDevice / Physical device used to run this Vulkan instance.
        - queue_indexes   / vector<uint32_t> / List containing the indexes of all queues to create.

    Returns:
        A vector list containing all queues create info.
*/
std::vector<VkDeviceQueueCreateInfo> Queues::make_queues_create_info
(
    const VkPhysicalDevice      &physical_device,
    const std::vector<uint32_t> &queue_indexes
)
{
    Utils::Logs::log("Making queue families create info.. ", false);

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical device invalid.");

    if (queue_indexes.size() < 1)
        Utils::Logs::crash_log("Failed! No queue family indexes provided.");

    std::set<uint32_t> unique_indexes = { queue_indexes.begin(), queue_indexes.end() }; // Prevent index duplication.
    std::vector<VkDeviceQueueCreateInfo> queues_create_info {};

    for (const uint32_t queue : unique_indexes)
    {
        /*
            - sType            / Defines the type of the structure.
            - queueFamilyIndex / Defines the family index of this queue.
            - queueCount       / Defines the amount of queues to create.
            - pQueuePriorities / Defines the priority of each queues.
        */
        const VkDeviceQueueCreateInfo create_info
        {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = queue,
            .queueCount = 1,
            .pQueuePriorities = (float*)1
        };

        queues_create_info.push_back(create_info);
    }

    Utils::Logs::log("Done!", true);
    return queues_create_info;
}
