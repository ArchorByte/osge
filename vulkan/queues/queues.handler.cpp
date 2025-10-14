#include "queues.handler.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <set>
#include <cstdint>

// Return the list of queue families available for a specific physical device.
std::vector<VkQueueFamilyProperties> get_queue_families
(
    const VkPhysicalDevice &physical_device
)
{
    log("Fetching the queue families list..");

    if (physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Queue families query failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    // Count the amount of queues available.
    uint32_t families_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &families_count, nullptr);

    if (families_count == 0)
    {
        fatal_error_log("Queue families query failed! No queue family was found on this physical device!");
    }

    // Register the queues into a list.
    std::vector<VkQueueFamilyProperties> queue_families(families_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &families_count, queue_families.data());

    if (queue_families.size() != families_count)
    {
        fatal_error_log("Queue families query failed! Some queue families are missing: " + std::to_string(families_count) + " queue families were found but " + std::to_string(queue_families.size()) + " queue families were collected!");
    }

    log(std::to_string(families_count) + " queue families were found!");
    return queue_families;
}

// Make and return the queues create info.
std::vector<VkDeviceQueueCreateInfo> make_queues_create_info
(
    const VkPhysicalDevice &physical_device,
    const std::vector<uint32_t> &required_queues_indexes,
    const float &queue_priority
)
{
    log("Making queue families create info..");

    if (physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Queue families create info failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (required_queues_indexes.size() < 1)
    {
        fatal_error_log("Queue families create info failed! No required queues families indexes were provided!");
    }

    std::set<uint32_t> unique_queues_families = { required_queues_indexes.begin(), required_queues_indexes.end() }; // Prevent indexes duplication.
    std::vector<VkDeviceQueueCreateInfo> queues_create_info {};

    for (const uint32_t queue : unique_queues_families)
    {
        VkDeviceQueueCreateInfo queue_create_info
        {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = queue,          // Index of the queue.
            .queueCount = 1,
            .pQueuePriorities = &queue_priority // Set the custom priority.
        };

        queues_create_info.push_back(queue_create_info);
    }

    log(std::to_string(unique_queues_families.size()) + " queue families create info created successfully!");
    return queues_create_info;
}
