#include "depth.resources.hpp"

#include "depth.formats.hpp"
#include "../images/image.transitions.hpp"
#include "../images/image.views.handler.hpp"
#include "../images/images.handler.hpp"
#include "../../helpers/help.text.format.hpp"
#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>
#include <utility>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create all resources for the depth buffering.
DepthResources create_depth_resources
(
    const VkPhysicalDevice &physical_device,
    const VkDevice &logical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkExtent2D &extent
)
{
    log("Creation depth resources..");

    if (physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Depth resources creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Depth resources creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (command_pool == VK_NULL_HANDLE)
    {
        fatal_error_log("Depth resources creation failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");
    }

    if (graphics_queue == VK_NULL_HANDLE)
    {
        fatal_error_log("Depth resources creation failed! The graphics queue provided (" + force_string(graphics_queue) + ") is not valid!");
    }

    const VkFormat depth_format = find_depth_format(physical_device);

    const std::pair<VkImage, VkDeviceMemory> depth_image = create_image
    (
        physical_device,
        logical_device,
        extent.width,
        extent.height,
        depth_format,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
    );

    const VkImageView image_view = create_image_view(logical_device, depth_image.first, depth_format, VK_IMAGE_ASPECT_DEPTH_BIT);
    transition_image_layout(logical_device, command_pool, graphics_queue, depth_image.first, depth_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    log("Depth resources created successfully!");
    return { depth_image.first, depth_image.second, image_view };
}

// Destroy the depth resources.
void destroy_depth_resources
(
    const VkDevice &logical_device,
    DepthResources &depth_resources
)
{
    log("Destroying depth resources..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Depth resources destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    VkImage depth_image = depth_resources.depth_image;
    VkDeviceMemory image_memory = depth_resources.image_memory;
    VkImageView image_view = depth_resources.image_view;

    if (depth_image == VK_NULL_HANDLE)
    {
        error_log("Depth resources destruction failed! The depth image provided (" + force_string(depth_image) + ") is not valid!");
        return;
    }

    if (image_memory == VK_NULL_HANDLE)
    {
        error_log("Depth resources destruction failed! The depth image memory provided (" + force_string(image_memory) + ") is not valid!");
        return;
    }

    if (image_view == VK_NULL_HANDLE)
    {
        error_log("Depth resources destruction failed! The depth image view provided (" + force_string(image_view) + ") is not valid!");
        return;
    }

    vkDestroyImage(logical_device, depth_image, nullptr);
    depth_image = VK_NULL_HANDLE;

    vkFreeMemory(logical_device, image_memory, nullptr);
    image_memory = VK_NULL_HANDLE;

    vkDestroyImageView(logical_device, image_view, nullptr);
    image_view = VK_NULL_HANDLE;

    log("Depth resources destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_DepthResources::Vulkan_DepthResources
(
    const VkPhysicalDevice &physical_device,
    const VkDevice &logical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkExtent2D &extent
) : logical_device(logical_device)
{
    depth_resources = create_depth_resources(physical_device, logical_device, command_pool, graphics_queue, extent);
}

// Destructor.
Vulkan_DepthResources::~Vulkan_DepthResources()
{
    destroy_depth_resources(logical_device, depth_resources);
}

DepthResources Vulkan_DepthResources::get() const
{
    return depth_resources;
}
