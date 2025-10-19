#include "logical.device.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a Vulkan logical device.
VkDevice create_logical_device
(
    const VkPhysicalDevice &physical_device,
    const std::vector<VkDeviceQueueCreateInfo> &queues_create_info,
    const std::vector<const char *> &required_extensions
)
{
    log("Creating a logical device..");

    if (physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Logical device creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (queues_create_info.size() < 1)
    {
        fatal_error_log("Logical device creation failed! No queues create info were provided!");
    }

    VkPhysicalDeviceFeatures device_features {};
    vkGetPhysicalDeviceFeatures(physical_device, &device_features);

    const VkDeviceCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = static_cast<uint32_t>(queues_create_info.size()),   // Amount of queues to create.
        .pQueueCreateInfos = queues_create_info.data(),                             // Pass the queues create info.
        .enabledExtensionCount = static_cast<uint32_t>(required_extensions.size()), // Amount of extensions to enable.
        .ppEnabledExtensionNames = required_extensions.data(),                      // Pass the required extensions list.
        .pEnabledFeatures = &device_features                                        // Enable all features of the physical device for simplicity and compatibility reasons.
    };

    VkDevice logical_device = VK_NULL_HANDLE;
    const VkResult device_creation = vkCreateDevice(physical_device, &create_info, nullptr, &logical_device);

    if (device_creation != VK_SUCCESS)
    {
        fatal_error_log("Logical device creation returned error code " + std::to_string(device_creation) + ".");
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Logical device creation output (" + force_string(logical_device) + ") is not valid!");
    }

    log("Logical device " + force_string(logical_device) + " created successfully!");
    return logical_device;
}

// Destroy a Vulkan logical device.
void destroy_logical_device
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

// Constructor.
Vulkan_LogicalDevice::Vulkan_LogicalDevice
(
    const VkPhysicalDevice &physical_device,
    const std::vector<VkDeviceQueueCreateInfo> &queues_create_info,
    const std::vector<const char *> &required_extensions
)
{
    logical_device = create_logical_device(physical_device, queues_create_info, required_extensions);
}

// Destructor.
Vulkan_LogicalDevice::~Vulkan_LogicalDevice()
{
    destroy_logical_device(logical_device);
}

VkDevice Vulkan_LogicalDevice::get() const
{
    return logical_device;
}
