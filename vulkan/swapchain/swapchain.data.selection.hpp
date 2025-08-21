#include <vulkan/vulkan.h>
#include <vector>
#include <SDL2/SDL.h>

#ifndef VULKAN_SWAPCHAIN_DATA_SELECTION_HPP
#define VULKAN_SWAPCHAIN_DATA_SELECTION_HPP

VkSurfaceFormatKHR select_best_vulkan_swapchain_surface_format
(
    const std::vector<VkSurfaceFormatKHR> &available_formats
);

VkPresentModeKHR select_best_vulkan_swapchain_present_mode
(
    const std::vector<VkPresentModeKHR> &available_present_modes
);

VkExtent2D select_vulkan_swapchain_extent_resolution
(
    const VkSurfaceCapabilitiesKHR &capabilities,
    SDL_Window* window
);

#endif
