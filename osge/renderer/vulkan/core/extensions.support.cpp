#include "vulkan.core.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <set>
#include <string>
#include <vector>

/*
    Check if some requested extensions are supported by the device.

    Tasks:
        1) Verify function parameters.
        2) Register all available extensions.
        3) Convert the extensions name from char to string.
        4) Verify if the requested extensions are supported, and display all unsupported extensions if there are any.

    Parameters:
        - physical_device     / VkPhysicalDevice     / Physical device used to run this Vulkan instance.
        - required_extensions / vector<const char *> / Extensions to check.

    Returns:
        A boolean that either confirms the extensions are supported or not.
*/
bool Core::check_extensions_support
(
    const VkPhysicalDevice &physical_device,
    const std::vector<const char *> &required_extensions
)
{
    Utils::Logs::log("Verifying support for " + std::to_string(required_extensions.size()) + " extensions.. ", false);

    if (physical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Physical device invalid.", true);
        return false;
    }

    if (required_extensions.size() < 1)
    {
        Utils::Logs::log("Done!", true);
        return true;
    }

    uint32_t extensions_count = 0;
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensions_count, nullptr);

    if (extensions_count < 1)
    {
        Utils::Logs::log("Failed! No extensions found on this device.", true);
        return false;
    }

    std::vector<VkExtensionProperties> available_extensions(extensions_count);
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensions_count, available_extensions.data());

    if (available_extensions.size() < 1)
    {
        Utils::Logs::log("Failed! No extensions retrieved.", true);
        return false;
    }

    std::set<std::string> required_extensions_list;

    for (const VkExtensionProperties &extension : available_extensions)
        required_extensions_list.erase(extension.extensionName);

    if (!required_extensions_list.empty())
    {
        Utils::Logs::log("Failed! " + std::to_string(required_extensions_list.size()) + " extensions are not supported.", true);

        for (const std::string &extension : required_extensions_list)
            Utils::Logs::log("- Extension \"" + extension + "\" NOT supported.", true);

        return false;
    }

    Utils::Logs::log("Done!", true);
    return true;
}
