#include "vulkan.swapchain.hpp"

#include "../../../utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"

#include <cstdint>
#include <string>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a swap chain.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Adjust create info depending on queue indexes.
        3) Create swap chain.

    Parameters:
        - capabilities          / VkSurfaceCapabilitiesKHR / Defines swap chain capabilities.
        - extent                / VkExtent2D               / Resolution of the swap chain to create.
        - graphics_family_index / uint32_t                 / Index of the graphics queue family.
        - images_count          / uint32_t                 / Defines the amount of swap chain images.
        - logical_device        / VkDevice                 / Logical device of the Vulkan instance.
        - present_family_index  / uint32_t                 / Index of the present queue family.
        - present_mode          / VkPresentModeKHR         / Defines how rendered images are presented.
        - surface_format        / VkSurfaceFormatKHR       / Defines the surface format to use for the swap chain.
        - vulkan_surface        / VkSurfaceKHR             / Link between this Vulkan instance and the SDL3 game window.

    Returns:
        The created swap chain.
*/
VkSwapchainKHR Swapchain::create_swapchain
(
    const VkSurfaceCapabilitiesKHR &capabilities,
    const VkExtent2D               &extent,
    const uint32_t                 &graphics_family_index,
    const uint32_t                 &images_count,
    const VkDevice                 &logical_device,
    const uint32_t                 &present_family_index,
    const VkPresentModeKHR         &present_mode,
    const VkSurfaceFormatKHR       &surface_format,
    const VkSurfaceKHR             &vulkan_surface
)
{
    Utils::Logs::log("Creating swap chain.. ", false);

    if (graphics_family_index < 0)
        Utils::Logs::crash_log("Failed! Graphics family index invalid -> " + std::to_string(graphics_family_index) + ".");

    if (images_count < 1)
        Utils::Logs::crash_log("Failed! Images count invalid -> " + std::to_string(images_count) + ".");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (present_family_index < 0)
        Utils::Logs::crash_log("Failed! Present family index invalid -> " + std::to_string(present_family_index) + ".");

    if (vulkan_surface == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Vulkan surface invalid.");

    const uint32_t queue_family_indices[] =
    {
        graphics_family_index,
        present_family_index
    };

    /*
        - sType            / Defines the type of the structure.
        - surface          / Defines where to present images.
        - minImageCount    / Defines the minimum amount of images needed by the engine.
        - imageFormat      / Defines the format of swap chain images.
        - imageColorSpace  / Defines how the swap chain interprets data.
        - imageExtent      / Defines the resolution of the swap chain.
        - imageArrayLayers / Defines the amount of layers in each image.
        - imageUsage       / Defines what we are going to do with swap chain images.
        - preTransform     / Defines transform to apply before presentation.
        - compositeAlpha   / Defines how alpha is handled.
        - presentMode      / Defines which present mode the swap chain is going to use.
        - clipped          / Defines whether the swap chain should ignore modifications out of bounds or not.
    */
    VkSwapchainCreateInfoKHR create_info
    {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = vulkan_surface,
        .minImageCount = images_count,
        .imageFormat = surface_format.format,
        .imageColorSpace = surface_format.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = present_mode,
        .clipped = VK_TRUE
    };

    if (graphics_family_index != present_family_index)
    {
        /*
            - imageSharingMode      / Defines whether swap chain images are shared or not.
            - queueFamilyIndexCount / Defines the amount of queue family indices to pass.
            - pQueueFamilyIndices   / Passes the indices.
        */
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    }
    else
    {
        create_info.imageSharingMode =  VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;
        create_info.pQueueFamilyIndices = nullptr;
    }

    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    const VkResult swapchain_creation = vkCreateSwapchainKHR(logical_device, &create_info, nullptr, &swapchain);

    if (swapchain_creation != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Creation returned error code -> " + std::to_string(swapchain_creation) + ".");

    Utils::Logs::log("Done! Memory address -> " + Utils::Text::get_memory_address(swapchain) + ".", true);
    return swapchain;
}



/*
    Destroy a swap chain.

    Tasks:
        1) Verify function parameters.
        2) Destroy swap chain.
        3) Set object to null.

    Parameters:
        - logical_device / VkDevice       / Logical device of the Vulkan instance.
        - swapchain      / VkSwapchainKHR / Swap chain to destroy.

    Returns:
        No object returned.
*/
void Swapchain::destroy_swapchain
(
    const VkDevice &logical_device,
    VkSwapchainKHR &swapchain
)
{
    Utils::Logs::log("Destroying " + Utils::Text::get_memory_address(swapchain) + " swap chain.. ", false);

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    if (swapchain == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Swap chain invalid.", true);
        return;
    }

    vkDestroySwapchainKHR(logical_device, swapchain, nullptr);
    swapchain = VK_NULL_HANDLE;

    Utils::Logs::log("Done!", true);
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Swapchain::swapchain_handler::swapchain_handler
(
    const VkSurfaceCapabilitiesKHR &capabilities,
    const VkExtent2D               &extent,
    const uint32_t                 &graphics_family_index,
    const uint32_t                 &images_count,
    const VkDevice                 &logical_device,
    const uint32_t                 &present_family_index,
    const VkPresentModeKHR         &present_mode,
    const VkSurfaceFormatKHR       &surface_format,
    const VkSurfaceKHR             &vulkan_surface
)
    : logical_device(logical_device)
{
    swapchain = create_swapchain(capabilities, extent, graphics_family_index, images_count, logical_device, present_family_index, present_mode, surface_format, vulkan_surface);
}

Swapchain::swapchain_handler::~swapchain_handler()
{
    destroy_swapchain(logical_device, swapchain);
}

VkSwapchainKHR Swapchain::swapchain_handler::get() const
{
    return swapchain;
}
