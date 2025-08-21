#include <vulkan/vulkan.h>
#include <vector>

#ifndef VULKAN_SWAPCHAIN_DATA_QUERIES_HPP
#define VULKAN_SWAPCHAIN_DATA_QUERIES_HPP

VkSurfaceCapabilitiesKHR get_vulkan_swapchain_capabilities
(
    const VkPhysicalDevice &physical_device,
    const VkSurfaceKHR &vulkan_surface
);

std::vector<VkSurfaceFormatKHR> get_vulkan_swapchain_surface_formats
(
    const VkPhysicalDevice &physical_device,
    const VkSurfaceKHR &vulkan_surface
);

std::vector<VkPresentModeKHR> get_vulkan_swapchain_present_modes
(
    const VkPhysicalDevice &physical_device,
    const VkSurfaceKHR &vulkan_surface
);

#endif
