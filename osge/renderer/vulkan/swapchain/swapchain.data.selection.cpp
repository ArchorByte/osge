#include "vulkan.swapchain.hpp"

#include "../../../utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"
#include "libraries/sdl/SDL3/SDL.h"
#include "libraries/sdl/SDL3/SDL_vulkan.h"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <vector>

/*
    Get the best swap chain surface format.

    Tasks:
        1) Verify function parameters.
        2) Browse the formats list and get the best format according to our requirements.
        3) Default to the first format in the list if none found.

    Parameters:
        - available_formats / vector<VkSurfaceFormatKHR> / Surface formats supported by the swap chain.

    Returns:
        The selected surface format.
*/
VkSurfaceFormatKHR Swapchain::select_best_swapchain_surface_format
(
    const std::vector<VkSurfaceFormatKHR> &formats
)
{
    Utils::Logs::log("Selecting best swap chain surface format.. ", false);

    if (formats.size() < 1)
        Utils::Logs::crash_log("Failed! No surface formats provided.");

    for (const VkSurfaceFormatKHR &format : formats)
    {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            Utils::Logs::log("Done!", true);
            return format;
        }
    }

    Utils::Logs::log("Done! Warning: First format selected as nothing better was found -> It might provokes some issues.", true);
    return formats[0];
}



/*
    Get the best swap chain present mode.

    Tasks:
        1) Verify function parameters.
        2) Browse the formats list and get the best mode according to our requirements.
        3) Default to VK_PRESENT_MODE_FIFO_KHR if none found.

    Parameters:
        - present_modes / vector<VkPresentModeKHR> / Present modes supported by the swap chain.

    Returns:
        The selected present mode.
*/
VkPresentModeKHR Swapchain::select_best_swapchain_present_mode
(
    const std::vector<VkPresentModeKHR> &present_modes
)
{
    Utils::Logs::log("Selecting best swap chain present mode.. ", false);

    for (const VkPresentModeKHR present_mode : present_modes)
    {
        if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR || present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
        {
            Utils::Logs::log("Done!", true);
            return present_mode;
        }
    }

    Utils::Logs::log("Done! Warning: VK_PRESENT_MODE_FIFO_KHR present mode selected as nothing better was found -> Vsync is forced on.", true);
    return VK_PRESENT_MODE_FIFO_KHR;
}



/*
    Set the swap chain resolution.

    Tasks:
        1) Verify function parameters.
        2) Get resolution of the game window.
        3) Set the swap chain resolution to game window resolution, checking it is supported by the swap chain.

    Parameters:
        - capabilities / VkSurfaceCapabilitiesKHR / Capabilities of the swap chain.
        - window       / SDL_Window*              / Window of the game.

    Returns:
        The swap chain extent.
*/
VkExtent2D Swapchain::select_swapchain_extent
(
    const VkSurfaceCapabilitiesKHR &capabilities,
    SDL_Window                     *window
)
{
    Utils::Logs::log("Selecting swap chain extent.. ", false);

    if (!window)
        Utils::Logs::crash_log("Failed! SDL3 window invalid.");

    int width;
    int height;

    SDL_GetWindowSizeInPixels(window, &width, &height);

    VkExtent2D extent =
    {
        std::clamp(static_cast<uint32_t>(width), capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
        std::clamp(static_cast<uint32_t>(height), capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
    };

    Utils::Logs::log("Done! Selected resolution -> " + std::to_string(extent.width) + "x" + std::to_string(extent.height) + ".", true);
    return extent;
}
