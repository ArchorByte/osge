#include "color.resources.hpp"

#include "../images/image.views.handler.hpp"
#include "../images/images.handler.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <utility>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create the resources necessary to create a color image view.
ColorResources create_color_resources
(
    const VkPhysicalDevice &physical_device,
    const VkDevice &logical_device,
    const VkExtent2D &swapchain_extent,
    const VkFormat &swapchain_image_format,
    const VkSampleCountFlagBits &samples_count
)
{
    log("Creating color resources..");

    if (physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Color resources creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Color resources creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    std::pair<VkImage, VkDeviceMemory> color_image_data = create_image(physical_device, logical_device, swapchain_extent.width, swapchain_extent.height, 1, samples_count, swapchain_image_format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    VkImage color_image = color_image_data.first;
    VkDeviceMemory color_image_memory = color_image_data.second;
    VkImageView color_image_view = create_image_view(logical_device, color_image, swapchain_image_format, VK_IMAGE_ASPECT_COLOR_BIT, 1);

    log("Color resources created successfully!");
    return { color_image, color_image_memory, color_image_view };
}

// Destroy color resources.
void destroy_color_resources
(
    const VkDevice &logical_device,
    ColorResources &color_resources
)
{
    log("Destroying color resources..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Color resources destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (color_resources.color_image_view == VK_NULL_HANDLE)
    {
        error_log("Warning: Color image view destruction failed! The image view provided (" + force_string(color_resources.color_image_view) + ") is not valid!");
    }
    else vkDestroyImageView(logical_device, color_resources.color_image_view, VK_NULL_HANDLE);

    if (color_resources.color_image == VK_NULL_HANDLE)
    {
        error_log("Warning: Color image destruction failed! The image provided (" + force_string(color_resources.color_image) + ") is not valid!");
    }
    else vkDestroyImage(logical_device, color_resources.color_image, VK_NULL_HANDLE);

    if (color_resources.color_image_memory == VK_NULL_HANDLE)
    {
        error_log("Warning: Color image memory destruction failed! The image memory provided (" + force_string(color_resources.color_image_memory) + ") is not valid!");
    }
    else vkFreeMemory(logical_device, color_resources.color_image_memory, VK_NULL_HANDLE);

    log("Color resources destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_ColorResources::Vulkan_ColorResources
(
    const VkPhysicalDevice &physical_device,
    const VkDevice &logical_device,
    const VkExtent2D &swapchain_extent,
    const VkFormat &swapchain_image_format,
    const VkSampleCountFlagBits &samples_count
) : logical_device(logical_device)
{
    color_resources = create_color_resources(physical_device, logical_device, swapchain_extent, swapchain_image_format, samples_count);
}

// Destructor.
Vulkan_ColorResources::~Vulkan_ColorResources()
{
    destroy_color_resources(logical_device, color_resources);
}

ColorResources Vulkan_ColorResources::get() const
{
    return color_resources;
}
