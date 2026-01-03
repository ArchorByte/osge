#include "vulkan.core.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <set>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

/*
    Check if some requested extensions are supported by the device.

    Tasks:
        1) Verify the parameters.
        2) Register all available extensions.
        3) Convert the extensions name from char to string.
        4) Verify the requested extensions are supported, and display all unsupported extensions if there are any.

    Parameters:
        - physical_device     / VkPhysicalDevice     / Physical device used to run Vulkan.
        - required_extensions / vector<const char *> / Requested extensions to check.

    Returns:
        A boolean that either confirms the extensions are supported or not.
*/
bool Vulkan::Core::check_extensions_support
(
    const VkPhysicalDevice &physical_device,
    const std::vector<const char *> &required_extensions
)
{
    log("Verifying Vulkan extensions support for " + std::to_string(required_extensions.size()) + " extensions..");

    if (physical_device == VK_NULL_HANDLE)
    {
        error_log("Vulkan extensions support verification failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
        return false;
    }

    if (required_extensions.size() < 1)
    {
        log("Vulkan extensions support verification succeeded! No extensions to verify, skipped!");
        return true;
    }

    uint32_t extensions_count = 0;
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensions_count, nullptr);

    if (extensions_count < 1)
    {
        error_log("Vulkan extensions support verification failed! No extensions found!");
        return false;
    }

    std::vector<VkExtensionProperties> available_extensions(extensions_count);
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensions_count, available_extensions.data());

    if (available_extensions.size() < 1)
    {
        error_log("Vulkan extensions support verification failed! No extensions retrieved!");
        return false;
    }

    std::set<std::string> required_extensions_list;

    for (const char* extension_name : required_extensions)
        required_extensions_list.insert(std::string(extension_name));

    for (const VkExtensionProperties &extension : available_extensions)
    {
        const bool succeeded = required_extensions_list.erase(extension.extensionName);

        if (succeeded)
            log("- Extension \"" + std::string(extension.extensionName) + "\" supported!");
    }

    if (!required_extensions_list.empty())
    {
        for (const std::string &extension : required_extensions_list)
            error_log("- Extension \"" + extension + "\" NOT supported!");

        error_log("Vulkan extensions support verification failed! " + std::to_string(required_extensions_list.size()) + " extensions aren't supported by the device!");
        return false;
    }

    log("Vulkan extensions support verification succeeded!");
    return true;
}
