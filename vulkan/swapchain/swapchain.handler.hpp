#include <vulkan/vulkan.h>
#include <cstdint>

#ifndef VULKAN_SWAPCHAIN_HPP
#define VULKAN_SWAPCHAIN_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

VkSwapchainKHR create_vulkan_swapchain
(
    const VkDevice &logical_device,
    const VkSurfaceCapabilitiesKHR &swapchain_capabilities,
    const VkPresentModeKHR &present_mode,
    const VkSurfaceKHR &vulkan_surface,
    const VkSurfaceFormatKHR &surface_format,
    const VkExtent2D &extent,
    const uint32_t &graphics_family_index,
    const uint32_t &present_family_index,
    const uint32_t &images_count
);

void destroy_vulkan_swapchain
(
    const VkDevice &logical_device,
    VkSwapchainKHR &swapchain
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_Swapchain
{

public:
    // Constructor.
    Vulkan_Swapchain
    (
        const VkDevice &logical_device,
        const VkSurfaceCapabilitiesKHR &swapchain_capabilities,
        const VkPresentModeKHR &present_mode,
        const VkSurfaceKHR &vulkan_surface,
        const VkSurfaceFormatKHR &surface_format,
        const VkExtent2D &extent,
        const uint32_t &graphics_family_index,
        const uint32_t &present_family_index,
        const uint32_t &images_count
    );

    // Destructor.
    ~Vulkan_Swapchain();

    VkSwapchainKHR get() const;

    // Prevent data duplication.
    Vulkan_Swapchain(const Vulkan_Swapchain&) = delete;
    Vulkan_Swapchain &operator = (const Vulkan_Swapchain&) = delete;

private:
    // We declare the members of the class to store.
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    VkDevice logical_device = VK_NULL_HANDLE;

};

#endif
