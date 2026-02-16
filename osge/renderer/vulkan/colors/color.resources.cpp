#include "vulkan.colors.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/renderer/vulkan/vulkan.renderer.hpp"
#include "osge/utils/utils.hpp"

#include <utility>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create the color image, the color image memory, and the color image view.
    Note: You should use the pre-made class to handle the color resources rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the parameters.
        2) Create the color image and allocate it some memory.
        3) Create the color image view.

    Parameters:
        - logical_device         / VkDevice              / Logical device of the Vulkan instance.
        - physical_device        / VkPhysicalDevice      / Physical device used to run the Vulkan instance.
        - samples_count          / VkSampleCountFlagBits / Amount of samples to generate for each frame for the multisampling.
        - swapchain_extent       / VkExtent2D            / Resolution of the swap chain.
        - swapchain_image_format / VkFormat              / Format of the swap chain images.

    Returns:
        A structure containing the color resources.
*/
ColorResources Colors::create_color_resources
(
    const VkDevice              &logical_device,
    const VkPhysicalDevice      &physical_device,
    const VkSampleCountFlagBits &samples_count,
    const VkExtent2D            &swapchain_extent,
    const VkFormat              &swapchain_image_format
)
{
    Utils::Logs::log("Creating color resources.. ", false);

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical device invalid.");

    const std::pair<VkImage, VkDeviceMemory> color_image_data = Vulkan::Images::create_image
    (
        swapchain_image_format, swapchain_extent.height, VK_IMAGE_TILING_OPTIMAL, logical_device, 1, physical_device, samples_count, swapchain_extent.width,
        VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
    );

    const VkImage color_image = color_image_data.first;
    const VkDeviceMemory color_image_memory = color_image_data.second;
    const VkImageView color_image_view = Vulkan::Images::create_image_view(VK_IMAGE_ASPECT_COLOR_BIT, swapchain_image_format, color_image, logical_device, 1);

    Utils::Logs::log("Done!", true);
    return { color_image, color_image_memory, color_image_view };
}



/*
    Cleanly destroy all color resources.

    Tasks:
        1) Verify function parameters.
        2) Destroy the objects.
        3) Set objects to null.

    Parameters:
        - color_resources / ColorResources / Color resources to destroy.
        - logical_device  / VkDevice       / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Colors::destroy_color_resources
(
    ColorResources &color_resources,
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Destroying color resources.. ", false);

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    if (color_resources.color_image_view == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Color image view invalid.", true);
        return;
    }

    vkDestroyImageView(logical_device, color_resources.color_image_view, VK_NULL_HANDLE);
    color_resources.color_image_view = VK_NULL_HANDLE;

    if (color_resources.color_image == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Color image invalid.", true);
        return;
    }

    vkDestroyImage(logical_device, color_resources.color_image, VK_NULL_HANDLE);
    color_resources.color_image = VK_NULL_HANDLE;

    if (color_resources.color_image_memory == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Color image memory invalid.", true);
        return;
    }

    vkFreeMemory(logical_device, color_resources.color_image_memory, VK_NULL_HANDLE);
    color_resources.color_image_memory = VK_NULL_HANDLE;

    Utils::Logs::log("Done!", true);
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Colors::color_resources_handler::color_resources_handler
(
    const VkDevice              &logical_device,
    const VkPhysicalDevice      &physical_device,
    const VkSampleCountFlagBits &samples_count,
    const VkExtent2D            &swapchain_extent,
    const VkFormat              &swapchain_image_format
)
    : logical_device(logical_device)
{
    color_resources = Colors::create_color_resources(logical_device, physical_device, samples_count, swapchain_extent, swapchain_image_format);
}

Colors::color_resources_handler::~color_resources_handler()
{
    Colors::destroy_color_resources(color_resources, logical_device);
}

ColorResources Colors::color_resources_handler::get() const
{
    return color_resources;
}
