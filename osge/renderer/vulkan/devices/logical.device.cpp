#include "vulkan.devices.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a logical device.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Retrieve all physical device features to enable them for simplicity and compatibility reasons.
        3) Create the logical device.

    Parameters:
        - physical_device     / VkPhysicalDevice                / Physical device used to run this Vulkan instance.
        - queues_create_info  / vector<VkDeviceQueueCreateInfo> / Information required for queues creation.
        - required_extensions / vector<const char *>            / List of extensions to enable.

    Returns:
        The created logical device.
*/
VkDevice Devices::create_logical_device
(
    const VkPhysicalDevice                     &physical_device,
    const std::vector<VkDeviceQueueCreateInfo> &queues_create_info,
    const std::vector<const char *>            &required_extensions
)
{
    Utils::Logs::log("Creating logical device.. ", false);

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical device invalid.");

    if (queues_create_info.size() < 1)
        Utils::Logs::crash_log("Failed! No queues create info provided.");

    VkPhysicalDeviceFeatures device_features { .sampleRateShading = VK_TRUE };
    vkGetPhysicalDeviceFeatures(physical_device, &device_features);

    /*
        - sType                   / Defines the type of the structure.
        - queueCreateInfoCount    / Defines the amount of queues create info we are going to pass.
        - pQueueCreateInfos       / Passes all information about the queues to create.
        - enabledExtensionCount   / Defines the amount of extensions to enable.
        - ppEnabledExtensionNames / Passes the name of all extensions to enable.
        - pEnabledFeatures        / Defines which physical device we have to enable.
    */
    const VkDeviceCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = static_cast<uint32_t>(queues_create_info.size()),
        .pQueueCreateInfos = queues_create_info.data(),
        .enabledExtensionCount = static_cast<uint32_t>(required_extensions.size()),
        .ppEnabledExtensionNames = required_extensions.data(),
        .pEnabledFeatures = &device_features
    };

    VkDevice logical_device = VK_NULL_HANDLE;
    const VkResult device_creation = vkCreateDevice(physical_device, &create_info, nullptr, &logical_device);

    if (device_creation != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Creation returned error code -> " + std::to_string(device_creation) + ".");

    Utils::Logs::log("Done! Memory address -> " + Utils::Text::get_memory_address(logical_device) + ".", true);
    return logical_device;
}



/*
    Destroy a logical device.

    Tasks:
        1) Verify function parameters.
        2) Destroy the logical device.
        3) Set object to null.

    Parameters:
        - logical_device / VkDevice / The logical device to destroy.

    Returns:
        No object returned.
*/
void Devices::destroy_logical_device
(
    VkDevice &logical_device
)
{
    Utils::Logs::log("Destroying logical device (" + Utils::Text::get_memory_address(logical_device) + ").. ", false);

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    vkDestroyDevice(logical_device, nullptr);
    logical_device = VK_NULL_HANDLE;

    Utils::Logs::log("Done!", true);
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Devices::logical_device_handler::logical_device_handler
(
    const VkPhysicalDevice                     &physical_device,
    const std::vector<VkDeviceQueueCreateInfo> &queues_create_info,
    const std::vector<const char *>            &required_extensions
)
{
    logical_device = create_logical_device(physical_device, queues_create_info, required_extensions);
}

Devices::logical_device_handler::~logical_device_handler()
{
    destroy_logical_device(logical_device);
}

VkDevice Devices::logical_device_handler::get() const
{
    return logical_device;
}
