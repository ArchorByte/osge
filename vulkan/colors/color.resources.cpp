#include "vulkan.colors.hpp"

#include "../images/image.views.handler.hpp"
#include "../images/images.handler.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <utility>
#include <vulkan/vulkan.h>

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
        - samples_count          / VkSampleCountFlagBits / Amount of samples to generate for each frame for the MSAA.
        - swapchain_extent       / VkExtent2D            / Resolution of the swap chain.
        - swapchain_image_format / VkFormat              / Format of the swap chain images.

    Returns:
        A structure containing the color resources.
*/
ColorResources Vulkan::Colors::create_color_resources
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkSampleCountFlagBits &samples_count,
    const VkExtent2D &swapchain_extent,
    const VkFormat &swapchain_image_format
)
{
    log("Creating color resources..");

    if (logical_device == VK_NULL_HANDLE)
        fatal_error_log("Color resources creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");

    if (physical_device == VK_NULL_HANDLE)
        fatal_error_log("Color resources creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");

    const std::pair<VkImage, VkDeviceMemory> color_image_data = create_image
    (
        physical_device, logical_device, swapchain_extent.width, swapchain_extent.height, 1, samples_count,
        swapchain_image_format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
    );

    const VkImage color_image = color_image_data.first;
    const VkDeviceMemory color_image_memory = color_image_data.second;
    const VkImageView color_image_view = create_image_view(logical_device, color_image, swapchain_image_format, VK_IMAGE_ASPECT_COLOR_BIT, 1);

    log("Color resources created successfully!");
    return { color_image, color_image_memory, color_image_view };
}



/*
    Cleanly destroy all color resources.

    Tasks:
        1) Verify the parameters.
        2) Destroy the objects and get rid their memory addresses if possible.

    Parameters:
        - color_resources / ColorResources / Color resources to destroy.
        - logical_device  / VkDevice       / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Vulkan::Colors::destroy_color_resources
(
    ColorResources &color_resources,
    const VkDevice &logical_device
)
{
    log("Destroying color resources..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Color resources destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (color_resources.color_image_view == VK_NULL_HANDLE)
        error_log("Warning: Color image view destruction failed! The image view provided (" + force_string(color_resources.color_image_view) + ") is not valid!");
    else
    {
        vkDestroyImageView(logical_device, color_resources.color_image_view, VK_NULL_HANDLE);
        color_resources.color_image_view = VK_NULL_HANDLE;
    }

    if (color_resources.color_image == VK_NULL_HANDLE)
        error_log("Warning: Color image destruction failed! The image provided (" + force_string(color_resources.color_image) + ") is not valid!");
    else
    {
        vkDestroyImage(logical_device, color_resources.color_image, VK_NULL_HANDLE);
        color_resources.color_image = VK_NULL_HANDLE;
    }

    if (color_resources.color_image_memory == VK_NULL_HANDLE)
        error_log("Warning: Color image memory destruction failed! The image memory provided (" + force_string(color_resources.color_image_memory) + ") is not valid!");
    else
    {
        vkFreeMemory(logical_device, color_resources.color_image_memory, VK_NULL_HANDLE);
        color_resources.color_image_memory = VK_NULL_HANDLE;
    }

    log("Color resources destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Vulkan::Colors::color_resources_handler::color_resources_handler
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkSampleCountFlagBits &samples_count,
    const VkExtent2D &swapchain_extent,
    const VkFormat &swapchain_image_format
)
    : logical_device(logical_device)
{
    color_resources = Vulkan::Colors::create_color_resources(logical_device, physical_device, samples_count, swapchain_extent, swapchain_image_format);
}

Vulkan::Colors::color_resources_handler::~color_resources_handler()
{
    Vulkan::Colors::destroy_color_resources(color_resources, logical_device);
}

ColorResources Vulkan::Colors::color_resources_handler::get() const
{
    return color_resources;
}
