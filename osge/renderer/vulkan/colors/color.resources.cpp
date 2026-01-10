#include "vulkan.colors.hpp"
#include "osge/renderer/vulkan/images/vulkan.images.hpp"
#include "osge/utils/utils.hpp"
#include <libraries/vulkan/vulkan.h>
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
    Utils::Logs::log("Creating color resources..");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Color resources creation failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Color resources creation failed! The physical device provided (" + Utils::Text::get_memory_address(physical_device) + ") is not valid!");

    const std::pair<VkImage, VkDeviceMemory> color_image_data = Vulkan::Images::create_image
    (
        swapchain_image_format, swapchain_extent.height, VK_IMAGE_TILING_OPTIMAL, logical_device, 1, physical_device, samples_count, swapchain_extent.width,
        VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
    );

    const VkImage color_image = color_image_data.first;
    const VkDeviceMemory color_image_memory = color_image_data.second;
    const VkImageView color_image_view = Vulkan::Images::create_image_view(VK_IMAGE_ASPECT_COLOR_BIT, swapchain_image_format, color_image, logical_device, 1);

    Utils::Logs::log("Color resources created successfully!");
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
    Utils::Logs::log("Destroying color resources..");

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Color resources destruction failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");
        return;
    }

    if (color_resources.color_image_view == VK_NULL_HANDLE)
        Utils::Logs::error_log("Warning: Color image view destruction failed! The image view provided (" + Utils::Text::get_memory_address(color_resources.color_image_view) + ") is not valid!");
    else
    {
        vkDestroyImageView(logical_device, color_resources.color_image_view, VK_NULL_HANDLE);
        color_resources.color_image_view = VK_NULL_HANDLE;
    }

    if (color_resources.color_image == VK_NULL_HANDLE)
        Utils::Logs::error_log("Warning: Color image destruction failed! The image provided (" + Utils::Text::get_memory_address(color_resources.color_image) + ") is not valid!");
    else
    {
        vkDestroyImage(logical_device, color_resources.color_image, VK_NULL_HANDLE);
        color_resources.color_image = VK_NULL_HANDLE;
    }

    if (color_resources.color_image_memory == VK_NULL_HANDLE)
        Utils::Logs::error_log("Warning: Color image memory destruction failed! The image memory provided (" + Utils::Text::get_memory_address(color_resources.color_image_memory) + ") is not valid!");
    else
    {
        vkFreeMemory(logical_device, color_resources.color_image_memory, VK_NULL_HANDLE);
        color_resources.color_image_memory = VK_NULL_HANDLE;
    }

    Utils::Logs::log("Color resources destroyed successfully!");
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
