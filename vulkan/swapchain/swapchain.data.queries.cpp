#include "swapchain.data.queries.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

// Return the capabilities of a swap chain.
VkSurfaceCapabilitiesKHR get_vulkan_swapchain_capabilities
(
    const VkPhysicalDevice &physical_device,
    const VkSurfaceKHR &vulkan_surface
)
{
    log("Fetching the swap chain capabilities..");

    if (!physical_device || physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain capabilities query failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (!vulkan_surface || vulkan_surface == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain capabilities query failed! The Vulkan surface provided (" + force_string(vulkan_surface) + ") is not valid!");
    }

    VkSurfaceCapabilitiesKHR swapchain_capabilities;
    VkResult query_result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, vulkan_surface, &swapchain_capabilities); // Try to get the capabilities.

    if (query_result != VK_SUCCESS)
    {
        fatal_error_log("Swap chain capabilites query returned error code " + std::to_string(query_result) + ".");
    }

    log("Swap chain capabilities retrieved successfully!");
    return swapchain_capabilities;
}

// Return every surface format supported by a swap chain.
std::vector<VkSurfaceFormatKHR> get_vulkan_swapchain_surface_formats
(
    const VkPhysicalDevice &physical_device,
    const VkSurfaceKHR &vulkan_surface
)
{
    log("Fetching the swap chain surface formats available..");

    if (!physical_device || physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain surface formats query failed! The physical provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (!vulkan_surface || vulkan_surface == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain surface formats query failed! The Vulkan surface provided (" + force_string(vulkan_surface) + ") is not valid!");
    }

    // Count the amount of surface formats available.
    uint32_t formats_count = 0;
    VkResult first_query = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, vulkan_surface, &formats_count, nullptr);

    if (first_query != VK_SUCCESS)
    {
        fatal_error_log("Swap chain surface formats query failed! Query 1/2 returned error code " + std::to_string(first_query) + ".");
    }

    if (formats_count < 1)
    {
        fatal_error_log("Swap chain surface formats query failed! No surface formats available found!");
    }

    // Register the surface formats into a list.
    std::vector<VkSurfaceFormatKHR> surface_formats(formats_count);
    VkResult second_query = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, vulkan_surface, &formats_count, surface_formats.data());

    if (second_query != VK_SUCCESS)
    {
        fatal_error_log("Swap chain surface formats query failed! Query 2/2 returned error code " + std::to_string(second_query) + ".");
    }

    log(std::to_string(surface_formats.size()) + " swap chain surface formats retrieved successfully!");
    return surface_formats;
}

// Return every present modes supported by a swap chain.
std::vector<VkPresentModeKHR> get_vulkan_swapchain_present_modes
(
    const VkPhysicalDevice &physical_device,
    const VkSurfaceKHR &vulkan_surface
)
{
    log("Fetching the swap chain present modes available..");

    if (!physical_device || physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain present modes query failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (!vulkan_surface || vulkan_surface == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain present modes query failed! The Vulkan surface provided (" + force_string(vulkan_surface) + ") is not valid!");
    }

    // Count the amount of present modes available.
    uint32_t modes_count = 0;
    VkResult first_query = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, vulkan_surface, &modes_count, nullptr); // Try to count the modes available.

    if (first_query != VK_SUCCESS)
    {
        fatal_error_log("Swap chain present modes query failed! Query 1/2 returned error code " + std::to_string(first_query) + ".");
    }

    if (modes_count < 1)
    {
        fatal_error_log("Swap chain present modes query failed! No present modes were found!");
    }

    // Register the present modes into a list.
    std::vector<VkPresentModeKHR> present_modes(modes_count);
    VkResult second_query = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, vulkan_surface, &modes_count, present_modes.data()); // Try to list the modes available

    if (second_query != VK_SUCCESS)
    {
        fatal_error_log("Swap chain present modes query failed! Query 2/2 returned error code " + std::to_string(second_query) + ".");
    }

    log(std::to_string(present_modes.size()) + " swap chain present modes retrieved successfully!");
    return present_modes;
}
