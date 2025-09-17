#include "vulkan.extensions.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <set>
#include <string>

// Check if a physical device handles every Vulkan extension listed.
void check_vulkan_extensions_support
(
    const VkPhysicalDevice &physical_device,
    const std::vector<const char *> &required_extensions
)
{
    log("Verifying Vulkan extensions compatibility for " + std::to_string(required_extensions.size()) + " extensions..");

    if (!physical_device || physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Vulkan extensions check up failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (required_extensions.size() < 1)
    {
        log("No required  extension provided! There is nothing to do.");
        log("The Vulkan extensions verification has ended successfully!");
        return;
    }

    // Count the amount of extensions supported by this device.
    uint32_t extensions_count = 0;
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensions_count, nullptr);

    // Register the supported extensions into a list.
    std::vector<VkExtensionProperties> available_extensions(extensions_count);
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensions_count, available_extensions.data());

    // Duplicate the required_extensions list into a new string set.
    std::set<std::string> required_extensions_list;

    // Convert each item from char to string.
    for (const char* extension_name : required_extensions)
    {
        required_extensions_list.insert(std::string(extension_name));
    }

    // Verify the extensions support by trying to empty the requirements list using the supported extensions list.
    for (const VkExtensionProperties &extension : available_extensions)
    {
        bool succeeded = required_extensions_list.erase(extension.extensionName);

        if (succeeded)
        {
            std::string string_extension_name = extension.extensionName;
            log("- Extension \"" + string_extension_name + "\" supported!");
        }
    }

    // If there are some extensions left in the string set, we display all of them because they are not supported.
    if (!required_extensions_list.empty())
    {
        for (const std::string &extension : required_extensions_list)
        {
            error_log("- Extension \"" + extension + "\" NOT supported!");
        }

        fatal_error_log("The extensions compatibility verification has failed! " + std::to_string(required_extensions_list.size()) + " extensions aren't supported by your device!");
    }

    log("The Vulkan extensions verification has ended successfully!");
}
