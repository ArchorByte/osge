#include "vulkan.swapchain.hpp"

#include "../../../utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"

#include <cstdint>
#include <vector>

/*
    Get the capabilities of the swap chain.

    Tasks:
        1) Verify function parameters.
        2) Get swap chain capabilities.

    Parameters:
        - physical_device / VkPhysicalDevice / Physical device used to run this Vulkan instance.
        - vulkan_surface  / VkSurfaceKHR     / Link between this Vulkan instance and the SDL3 game window.

    Returns:
        The retrieved swap chain capabilities.
*/
VkSurfaceCapabilitiesKHR Swapchain::get_swapchain_capabilities
(
    const VkPhysicalDevice &physical_device,
    const VkSurfaceKHR     &vulkan_surface
)
{
    Utils::Logs::log("Getting swap chain capabilities.. ", false);

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical device invalid.");

    if (vulkan_surface == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Vulkan surface invalid.");

    VkSurfaceCapabilitiesKHR swapchain_capabilities;
    const VkResult query_result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, vulkan_surface, &swapchain_capabilities);

    if (query_result != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Capabilites query returned error code -> " + std::to_string(query_result) + ".");

    Utils::Logs::log("Done!", true);
    return swapchain_capabilities;
}



/*
    Get surface formats supported by the Vulkan surface.

    Tasks:
        1) Verify function parameters.
        2) Retrieve surface formats.

    Parameters:
        - physical_device / VkPhysicalDevice / Physical device used to run this Vulkan instance.
        - vulkan_surface  / VkSurfaceKHR     / Link between this Vulkan instance and the SDL3 game window.

    Returns:
        A vector list containing all surface formats supported.
*/
std::vector<VkSurfaceFormatKHR> Swapchain::get_swapchain_surface_formats
(
    const VkPhysicalDevice &physical_device,
    const VkSurfaceKHR     &vulkan_surface
)
{
    Utils::Logs::log("Getting swap chain surface formats.. ", false);

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical provided invalid.");

    if (vulkan_surface == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Vulkan surface invalid.");

    uint32_t formats_count = 0;
    const VkResult first_query = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, vulkan_surface, &formats_count, nullptr);

    if (first_query != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! First query returned error code -> " + std::to_string(first_query) + ".");

    if (formats_count == 0)
        Utils::Logs::crash_log("Failed! No surface formats detected.");

    std::vector<VkSurfaceFormatKHR> surface_formats(formats_count);
    const VkResult second_query = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, vulkan_surface, &formats_count, surface_formats.data());

    if (second_query != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Second query returned error code -> " + std::to_string(second_query) + ".");

    Utils::Logs::log("Done! " + std::to_string(surface_formats.size()) + " surface formats retrieved.", true);
    return surface_formats;
}



/*
    Get present modes supported by the Vulkan surface.

    Tasks:
        1) Verify function parameters.
        2) Retrieve present modes.

    Parameters:
        - physical_device / VkPhysicalDevice / Physical device used to run this Vulkan instance.
        - vulkan_surface  / VkSurfaceKHR     / Link between this Vulkan instance and the SDL3 game window.

    Returns:
        A vector list containing all present modes supported.
*/
std::vector<VkPresentModeKHR> Swapchain::get_swapchain_present_modes
(
    const VkPhysicalDevice &physical_device,
    const VkSurfaceKHR     &vulkan_surface
)
{
    Utils::Logs::log("Fetching swap chain present modes.. ", false);

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical device invalid.");

    if (vulkan_surface == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Vulkan surface invalid.");

    uint32_t modes_count = 0;
    const VkResult first_query = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, vulkan_surface, &modes_count, nullptr);

    if (first_query != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! First query returned error code -> " + std::to_string(first_query) + ".");

    if (modes_count < 1)
        Utils::Logs::crash_log("Failed! No present modes detected.");

    std::vector<VkPresentModeKHR> present_modes(modes_count);
    const VkResult second_query = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, vulkan_surface, &modes_count, present_modes.data());

    if (second_query != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Second query returned error code -> " + std::to_string(second_query) + ".");

    Utils::Logs::log("Done! " + std::to_string(present_modes.size()) + " present modes retrieved.", true);
    return present_modes;
}
