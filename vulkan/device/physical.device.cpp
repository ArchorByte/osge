#include "physical.device.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

// Check if a physical device meets our requirements.
bool is_valid_physical_device
(
    const VkPhysicalDevice &physical_device
)
{
    if (physical_device == VK_NULL_HANDLE)
    {
        error_log("Physical device validation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
        return false;
    }

    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(physical_device, &device_properties);

    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(physical_device, &device_features);

    return device_features.geometryShader;
}

// Return the name of a physical device.
std::string get_physical_device_name
(
    const VkPhysicalDevice &physical_device
)
{
    if (physical_device == VK_NULL_HANDLE)
    {
        error_log("Invalid physical device provided (" + force_string(physical_device) + ")! Returned \"Unknown GPU\".");
        return "Unknown GPU";
    }

    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(physical_device, &device_properties);

    return device_properties.deviceName;
}

// Return a usable physical device.
VkPhysicalDevice select_physical_device
(
    const VkInstance &vulkan_instance
)
{
    log("Looking for a usable physical device..");

    if (vulkan_instance == VK_NULL_HANDLE)
    {
        fatal_error_log("Physical device search failed! The Vulkan instance provided (" + force_string(vulkan_instance) + ") is not valid!");
    }

    // Count the amount of physical devices available.
    uint32_t devices_count = 0;
    VkResult first_devices_query = vkEnumeratePhysicalDevices(vulkan_instance, &devices_count, nullptr);

    if (first_devices_query != VK_SUCCESS)
    {
        fatal_error_log("Physical device search failed! The physical devices query 1/2 returned error code " + std::to_string(first_devices_query) + ".");
    }

    if (devices_count == 0)
    {
        fatal_error_log("Physical device search failed! Failed to find any physical device with a driver supporting Vulkan!");
    }

    // Register the physical devices available into a list.
    std::vector<VkPhysicalDevice> devices_list(devices_count);
    VkResult second_devices_query = vkEnumeratePhysicalDevices(vulkan_instance, &devices_count, devices_list.data());

    if (second_devices_query != VK_SUCCESS)
    {
        fatal_error_log("Physical device search failed! Physical devices query 2/2 returned error code " + std::to_string(second_devices_query) + ".");
    }

    VkPhysicalDevice physical_device = VK_NULL_HANDLE;

    for (const VkPhysicalDevice &device : devices_list)
    {
        if (is_valid_physical_device(device))
        {
            // We take the first valid physical device.
            physical_device = device;
            break;
        }
    }

    if (physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Physical device search failed! Failed to find any usable physical device!");
    }

    log("Physical device selected successfully! Selected device: " + get_physical_device_name(physical_device));
    return physical_device;
}
