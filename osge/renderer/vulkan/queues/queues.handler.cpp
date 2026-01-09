#include "vulkan.queues.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <set>
#include <vector>
#include <vulkan/vulkan.h>

/*
    Get all available queue families for a physical device.

    Tasks:
        1) Verify the parameters.
        2) Retrieve all queues.

    Parameters:
        - physical_device / VkPhysicalDevice / Physical device used to run Vulkan.

    Returns:
        A vector list containing all queue families.
*/
std::vector<VkQueueFamilyProperties> Vulkan::Queues::get_queue_families
(
    const VkPhysicalDevice &physical_device
)
{
    log("Fetching the queue families list..");

    if (physical_device == VK_NULL_HANDLE)
        fatal_error_log("Queue families query failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");

    uint32_t families_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &families_count, nullptr);

    if (families_count == 0)
        fatal_error_log("Queue families query failed! No queue family was found on this physical device!");

    std::vector<VkQueueFamilyProperties> queue_families(families_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &families_count, queue_families.data());

    if (queue_families.size() != families_count)
        fatal_error_log("Queue families query failed! Some queue families are missing: " + std::to_string(families_count) + " queue families were found but " + std::to_string(queue_families.size()) + " queue families were collected!");

    log(std::to_string(families_count) + " queue families were found!");
    return queue_families;
}



/*
    Make all queues create info.

    Tasks:
        1) Verify the parameters.
        2) Make a create info for each queue family.

    Parameters:
        - physical_device        / VkPhysicalDevice / Physical device used to run Vulkan.
        - queue_priority         / float            / Custom queue priority.
        - required_queue_indexes / vector<uint32_t> / Indexes of all required queues.

    Returns:
        A vector list containing all queues create info.
*/
std::vector<VkDeviceQueueCreateInfo> Vulkan::Queues::make_queues_create_info
(
    const VkPhysicalDevice &physical_device,
    const float &queue_priority,
    const std::vector<uint32_t> &required_queue_indexes
)
{
    log("Making queue families create info..");

    if (physical_device == VK_NULL_HANDLE)
        fatal_error_log("Queue families create info making failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");

    if (required_queue_indexes.size() < 1)
        fatal_error_log("Queue families create info making failed! No required queue family indexes provided!");

    std::set<uint32_t> unique_queue_families = { required_queue_indexes.begin(), required_queue_indexes.end() }; // Prevent index duplication.
    std::vector<VkDeviceQueueCreateInfo> queues_create_info {};

    for (const uint32_t queue : unique_queue_families)
    {
        const VkDeviceQueueCreateInfo create_info
        {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = queue,
            .queueCount = 1,
            .pQueuePriorities = &queue_priority
        };

        queues_create_info.push_back(create_info);
    }

    log(std::to_string(unique_queue_families.size()) + " queue families create info made successfully!");
    return queues_create_info;
}
