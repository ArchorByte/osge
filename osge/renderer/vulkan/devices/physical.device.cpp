#include "vulkan.devices.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <string>
#include <vector>

/*
    Check if a physical device meets our requirements.

    Tasks:
        1) Verify function parameters.
        2) Retrieve device information.
        3) Check if it meets our requirements.

    Parameters:
        - physical_device / VkPhysicalDevice / Physical device that we need to verify.

    Returns:
        A boolean informing whether the device meets our requirements or not.
*/
bool Devices::is_valid_physical_device
(
    const VkPhysicalDevice &physical_device
)
{
    if (physical_device == VK_NULL_HANDLE)
        return false;

    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(physical_device, &device_properties);

    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(physical_device, &device_features);

    const int valid = device_features.geometryShader;
    return valid;
}



/*
    Get and return the name of a physical device.

    Tasks:
        1) Verify function parameters.
        2) Retrieve the name of the device through its properties.

    Parameters:
        - physical_device / VkPhysicalDevice / Physical device to get the name from.

    Returns:
        A string containing the physical device name.
*/
std::string Devices::get_physical_device_name
(
    const VkPhysicalDevice &physical_device
)
{
    if (physical_device == VK_NULL_HANDLE)
        return "Unknown GPU";

    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(physical_device, &device_properties);

    const std::string device_name = device_properties.deviceName;
    return device_name;
}



/*
    Select a physical device that meets our requirements.

    Tasks:
        1) Verify function parameters.
        2) Get a list of all available physical devices.
        3) Select a suitable device that corresponds to our requirements, and the index requested.

    Parameters:
        - selected_device_index / int        / Index of the desired device.
        - vulkan_instance       / VkInstance / Current vulkan instance.

    Returns:
        The selected physical device.
*/
VkPhysicalDevice Devices::select_physical_device
(
    int              &selected_device_index,
    const VkInstance &vulkan_instance
)
{
    Utils::Logs::log("Looking for a suitable physical device.. ", false);

    if (vulkan_instance == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Physical device selection failed! The Vulkan instance provided (" + Utils::Text::get_memory_address(vulkan_instance) + ") is not valid!");

    uint32_t devices_count = 0;
    const VkResult first_query = vkEnumeratePhysicalDevices(vulkan_instance, &devices_count, nullptr);

    if (first_query != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Query 1/2 returned error code -> " + std::to_string(first_query) + ".");

    if (devices_count == 0)
        Utils::Logs::crash_log("Failed! No physical device supporting Vulkan found.");

    if (selected_device_index > devices_count)
    {
        Utils::Logs::log("[Warning: Selected device out of bounds -> " + std::to_string(selected_device_index) + ".] ", false);
        selected_device_index = 1;
    }

    std::vector<VkPhysicalDevice> devices_list(devices_count);
    const VkResult second_query = vkEnumeratePhysicalDevices(vulkan_instance, &devices_count, devices_list.data());

    if (second_query != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Query 2/2 returned error code -> " + std::to_string(second_query) + ".");

    if (devices_list.size() < 1)
        Utils::Logs::crash_log("Failed! No physical devices available.");

    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    int i = 0;

    for (const VkPhysicalDevice &device : devices_list)
    {
        i++;

        if (is_valid_physical_device(device) && i == selected_device_index)
        {
            physical_device = device;
            break;
        }
    }

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! No suitable physical device found.");

    Utils::Logs::log("Done! Selected device -> " + get_physical_device_name(physical_device) + ".", true);
    return physical_device;
}
