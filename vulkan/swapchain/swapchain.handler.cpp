#include "swapchain.handler.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <cstdint>
#include <string>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a swap chain.
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
)
{
    log("Creating a swap chain..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (!vulkan_surface || vulkan_surface == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain creation failed! The Vulkan surface provided (" + force_string(vulkan_surface) + ") is not valid!");
    }

    if (graphics_family_index < 0)
    {
        fatal_error_log("Swap chain creation failed! The graphics family index provided (" + std::to_string(graphics_family_index) + ") is not valid!");
    }

    if (present_family_index < 0)
    {
        fatal_error_log("Swap chain creation failed! The present family index provided (" + std::to_string(present_family_index) + ") is not valid!");
    }

    if (images_count < 1)
    {
        fatal_error_log("Swap chain creation failed! The images count provided (" + std::to_string(images_count) + ") is not valid!");
    }

    // Create a list with the queue family indexes.
    uint32_t queue_family_indices[] =
    {
        graphics_family_index,
        present_family_index
    };

    // Create info for the swap chain.
    VkSwapchainCreateInfoKHR info {};
    info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    info.surface = vulkan_surface;                               // Pass the Vulkan surface.
    info.minImageCount = images_count;                           // Minimum amount allowed of images in the swap chain.
    info.imageFormat = surface_format.format;                    // Set the required format for the images.
    info.imageColorSpace = surface_format.colorSpace;            // Color space of the swap chain.
    info.imageExtent = extent;                                   // Pass the extent.
    info.imageArrayLayers = 1;                                   // Amount of layers per image.
    info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;       // Use the images as color attachments.
    info.preTransform = swapchain_capabilities.currentTransform; // Specify any image transform (position, rotation..).
    info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;     // Ignore the alpha channel.
    info.presentMode = present_mode;                             // Pass the present mode.
    info.clipped = VK_TRUE;                                      // Disallow the render of any pixel out of bounds.
    info.oldSwapchain = VK_NULL_HANDLE;                          // Not used. Simplify the swap chain recreation process.

    // Set up the sharing mode depending on the queue family indexes.
    if (graphics_family_index != present_family_index)
    {
        info.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // Allow images sharing without ownership transfers.
        info.queueFamilyIndexCount = 2;                     // Amount of queue families that will be allowed to share images.
        info.pQueueFamilyIndices = queue_family_indices;    // Pass the queue family indexes.
    }
    else
    {
        info.imageSharingMode =  VK_SHARING_MODE_EXCLUSIVE; // Disallow images sharing without ownership transfers.
        info.queueFamilyIndexCount = 0;                     // Same here.
        info.pQueueFamilyIndices = nullptr;                 // Same here.
    }

    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    VkResult swapchain_creation = vkCreateSwapchainKHR(logical_device, &info, nullptr, &swapchain); // Try to create the swap chain.

    if (swapchain_creation != VK_SUCCESS)
    {
        fatal_error_log("Swap chain creation returned error code " + std::to_string(swapchain_creation) + ".");
    }

    if (!swapchain || swapchain == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain creation output \"" + force_string(swapchain) + "\" is not valid!");
    }

    log("Swap chain " + force_string(swapchain) + " created successfully!");
    return swapchain;
}

// Destroy a swap chain.
void destroy_vulkan_swapchain
(
    const VkDevice &logical_device,
    VkSwapchainKHR &swapchain
)
{
    log("Destroying the " + force_string(swapchain) + " swap chain..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        error_log("Swap chain destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (!swapchain || swapchain == VK_NULL_HANDLE)
    {
        error_log("Swap chain destruction failed! The swapchain provided (" + force_string(swapchain) + ") is not valid!");
        return;
    }

    // Destroy the swap chain and dispose of the memory.
    vkDestroySwapchainKHR(logical_device, swapchain, nullptr);
    swapchain = VK_NULL_HANDLE;

    log("Swap chain destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_Swapchain::Vulkan_Swapchain
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
) : logical_device(logical_device)
{
    swapchain = create_vulkan_swapchain(logical_device, swapchain_capabilities, present_mode, vulkan_surface, surface_format, extent, graphics_family_index, present_family_index, images_count);
}

// Destructor.
Vulkan_Swapchain::~Vulkan_Swapchain()
{
    destroy_vulkan_swapchain(logical_device, swapchain);
}

VkSwapchainKHR Vulkan_Swapchain::get() const
{
    return swapchain;
}
