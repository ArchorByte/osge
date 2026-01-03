#include "vulkan.devices.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vector>
#include <vulkan/vulkan.h>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a logical device.
    Note: You should use the pre-made class to handle the logical device rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the parameters.
        2) Retrieve all physical device features to enable them for simplicity and compatibility reasons.
        3) Create the logical device.

    Parameters:
        - physical_device     / VkPhysicalDevice                / Physical device used to run Vulkan.
        - queues_create_info  / vector<VkDeviceQueueCreateInfo> / All create info for the queues we are going to create.
        - required_extensions / vector<const char *>            / List of extensions that we wish to enable.

    Returns:
        The created logical device.
*/
VkDevice Vulkan::Devices::create_logical_device
(
    const VkPhysicalDevice &physical_device,
    const std::vector<VkDeviceQueueCreateInfo> &queues_create_info,
    const std::vector<const char *> &required_extensions
)
{
    log("Creating a logical device..");

    if (physical_device == VK_NULL_HANDLE)
        fatal_error_log("Logical device creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");

    if (queues_create_info.size() < 1)
        fatal_error_log("Logical device creation failed! No queues create info provided!");

    VkPhysicalDeviceFeatures device_features { .sampleRateShading = VK_TRUE };
    vkGetPhysicalDeviceFeatures(physical_device, &device_features);

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
        fatal_error_log("Logical device creation returned error code " + std::to_string(device_creation) + ".");

    log("Logical device " + force_string(logical_device) + " created successfully!");
    return logical_device;
}



/*
    Destroy a logical device.

    Tasks:
        1) Verify the parameters.
        2) Destroy the logical device.
        3) Replace the object address.

    Parameters:
        - logical_device / VkDevice / The logical device to destroy.

    Returns:
        No object returned.
*/
void Vulkan::Devices::destroy_logical_device
(
    VkDevice &logical_device
)
{
    log("Destroying the " + force_string(logical_device) + " logical device..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Logical device destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    vkDestroyDevice(logical_device, nullptr);
    logical_device = VK_NULL_HANDLE;

    log("Logical device destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Vulkan::Devices::logical_device_handler::logical_device_handler
(
    const VkPhysicalDevice &physical_device,
    const std::vector<VkDeviceQueueCreateInfo> &queues_create_info,
    const std::vector<const char *> &required_extensions
)
{
    logical_device = Vulkan::Devices::create_logical_device(physical_device, queues_create_info, required_extensions);
}

Vulkan::Devices::logical_device_handler::~logical_device_handler()
{
    Vulkan::Devices::destroy_logical_device(logical_device);
}

VkDevice Vulkan::Devices::logical_device_handler::get() const
{
    return logical_device;
}
