#include "vulkan.devices.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

/*
    Check if a physical device meets our requirements.

    Tasks:
        1) Verify the parameters.
        2) Retrieve device information.
        3) Check if it meets the requirements.

    Parameters:
        - physical_device / VkPhysicalDevice / Targeted device.

    Returns:
        A boolean informing whether the device meets the requirements or not.
*/
bool Vulkan::Devices::is_valid_physical_device
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

    const int valid = device_features.geometryShader;
    return valid;
}



/*
    Get and return the name of a physical device.

    Tasks:
        1) Verify the parameters.
        2) Retrieve the name of the device through its properties.

    Parameters:
        - physical_device / VkPhysicalDevice / Targeted device.

    Returns:
        A string containing the physical device name.
*/
std::string Vulkan::Devices::get_physical_device_name
(
    const VkPhysicalDevice &physical_device
)
{
    if (physical_device == VK_NULL_HANDLE)
    {
        error_log("Invalid physical device provided (" + force_string(physical_device) + ")! Defaulted to \"Unknown GPU\".");
        return "Unknown GPU";
    }

    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(physical_device, &device_properties);

    const std::string device_name = device_properties.deviceName;
    return device_name;
}



/*
    Select a physical device that meets our requirements.

    Tasks:
        1) Verify the parameters.
        2) Get all available physical devices.
        3) Select a suitable device that corresponds to our requirements.

    Parameters:
        - selected_device_index / int        / Index of the desired device.
        - vulkan_instance       / VkInstance / Vulkan instance.

    Returns:
        The selected physical device.
*/
VkPhysicalDevice Vulkan::Devices::select_physical_device
(
    int &selected_device_index,
    const VkInstance &vulkan_instance
)
{
    log("Looking for a usable physical device..");

    if (vulkan_instance == VK_NULL_HANDLE)
        fatal_error_log("Physical device selection failed! The Vulkan instance provided (" + force_string(vulkan_instance) + ") is not valid!");

    uint32_t devices_count = 0;
    const VkResult first_query = vkEnumeratePhysicalDevices(vulkan_instance, &devices_count, nullptr);

    if (first_query != VK_SUCCESS)
        fatal_error_log("Physical device selection failed! The physical device query 1/2 returned error code " + std::to_string(first_query) + ".");

    if (devices_count == 0)
        fatal_error_log("Physical device selection failed! Failed to find any physical device supporting Vulkan!");

    if (selected_device_index > devices_count)
    {
        error_log("Warning: The selected device (" + std::to_string(selected_device_index) + ") is out of bounds! Defaulted to the first valid physical device found!");
        selected_device_index = 1;
    }

    std::vector<VkPhysicalDevice> devices_list(devices_count);
    const VkResult second_query = vkEnumeratePhysicalDevices(vulkan_instance, &devices_count, devices_list.data());

    if (second_query != VK_SUCCESS)
        fatal_error_log("Physical device selection failed! Physical device query 2/2 returned error code " + std::to_string(second_query) + ".");

    if (devices_list.size() < 1)
        fatal_error_log("Physical device selection failed! No physical devices available!");

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
        fatal_error_log("Physical device selection failed! Failed to find any suitable physical device!");

    log("Physical device selected successfully! Selected device: " + get_physical_device_name(physical_device) + ".");
    return physical_device;
}
