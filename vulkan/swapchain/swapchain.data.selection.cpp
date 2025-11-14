#include "swapchain.data.selection.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vector>
#include <cstdint>
#include <limits>
#include <algorithm>

// Return the best swap chain surface format available, according to our requirements.
VkSurfaceFormatKHR select_best_vulkan_swapchain_surface_format
(
    const std::vector<VkSurfaceFormatKHR> &available_formats
)
{
    log("Selecting the best swap chain surface format..");

    for (const VkSurfaceFormatKHR &format : available_formats)
    {
        // Try to find a format meeting our requirements.
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            log("Best swap chain surface format selected successfully!");
            return format;
        }
    }

    error_log("Warning: We had to select the first result because not any better format was found! This might provokes some unexpected results!");
    return available_formats[0]; // We return the first format available if we didn't find any format meeting our requirements.
}

// Select the best swap chain present mode available, according to our requirements.
VkPresentModeKHR select_best_vulkan_swapchain_present_mode
(
    const std::vector<VkPresentModeKHR> &available_present_modes
)
{
    log("Selecting the best swap chain present mode..");

    for (const VkPresentModeKHR present_mode : available_present_modes)
    {
        // Try to find a mode with the "VK_PRESENT_MODE_MAILBOX_KHR" or "VK_PRESENT_MODE_IMMEDIATE_KHR" flag.
        if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR || present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
        {
            log("Best swap chain present mode selected successfully!!");
            return present_mode;
        }
    }

    log("Warning: We had to select the \"VK_PRESENT_MODE_FIFO_KHR\" present mode because not any better mode was found! That means the vsync is forced to be turned on!");
    return VK_PRESENT_MODE_FIFO_KHR; // Return this flag (which always exists), if we didn't find any usable mode with our requirements.
}

// Select the resolution for the swap chain extent.
VkExtent2D select_vulkan_swapchain_extent_resolution
(
    const VkSurfaceCapabilitiesKHR &capabilities,
    SDL_Window* window
)
{
    log("Selecting swap chain extent resolution..");

    if (!window)
    {
        fatal_error_log("Swap chain resolution selection failed! The SDL3 window provided (" + force_string(window) + ") is not valid!");
    }

    int width;
    int height;
    SDL_GetWindowSizeInPixels(window, &width, &height);

    VkExtent2D extent =
    {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    // Ensure that the extent resolution is within the swap chain capabilities bounds.
    extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    log("Swap chain extent resolution selected: " + std::to_string(extent.width) + "x" + std::to_string(extent.height) + ".");
    return extent;
}
