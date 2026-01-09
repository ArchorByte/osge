#include "vulkan.depth.hpp"

#include "../images/image.transitions.hpp"
#include "../images/image.views.handler.hpp"
#include "../images/images.handler.hpp"
#include "../../utils/tool.text.format.hpp"
#include "../../logs/logs.handler.hpp"

#include <utility>
#include <vulkan/vulkan.h>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create the depth image, depth image memory and depth image view.
    Note: You should use the pre-made class to handle the depth resources rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the parameters.
        2) Get the depth format.
        3) Create the depth image (image + image memory).
        4) Create the depth image view.
        5) Change the image layout from undefined to VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL.

    Parameters:
        - command_pool    / VkCommandPool         / Command pool of the Vulkan instance.
        - extent          / VkExtent2D            / Resolution of the swap chain.
        - graphics_queue  / VkQueue               / Graphics queue of the Vulkan instance.
        - logical_device  / VkDevice              / Logical device of the Vulkan instance.
        - physical_device / VkPhysicalDevice      / Physical device used to run Vulkan.
        - samples_count   / VkSampleCountFlagBits / Amount of samples to render at the same time for multisampling.

    Returns:
        The created depth resources.
*/
DepthResources Vulkan::Depth::create_depth_resources
(
    const VkCommandPool &command_pool,
    const VkExtent2D &extent,
    const VkQueue &graphics_queue,
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkSampleCountFlagBits &samples_count
)
{
    log("Creation depth resources..");

    if (command_pool == VK_NULL_HANDLE)
        fatal_error_log("Depth resources creation failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");

    if (graphics_queue == VK_NULL_HANDLE)
        fatal_error_log("Depth resources creation failed! The graphics queue provided (" + force_string(graphics_queue) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        fatal_error_log("Depth resources creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");

    if (physical_device == VK_NULL_HANDLE)
        fatal_error_log("Depth resources creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");

    const VkFormat depth_format = Vulkan::Depth::find_depth_format(physical_device);
    const std::pair<VkImage, VkDeviceMemory> depth_image = create_image(physical_device, logical_device, extent.width, extent.height, 1, samples_count, depth_format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
    const VkImageView image_view = create_image_view(logical_device, depth_image.first, depth_format, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
    transition_image_layout(logical_device, command_pool, graphics_queue, depth_image.first, depth_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);

    log("Depth resources created successfully!");
    return { depth_image.first, depth_image.second, image_view };
}



/*
    Cleanly destroy the depth resources.

    Tasks:
        1) Verify the parameters.
        2) Destroy the objects and get rid their memory addresses if possible.

    Parameters:
        - depth_resources / DepthResources / Depth resources to destroy.
        - logical_device  / VkDevice       / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Vulkan::Depth::destroy_depth_resources
(
    DepthResources &depth_resources,
    const VkDevice &logical_device
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
        error_log("Depth resources destruction failed! The depth image provided (" + force_string(depth_image) + ") is not valid!");
    else
    {
        vkDestroyImage(logical_device, depth_image, nullptr);
        depth_image = VK_NULL_HANDLE;
    }

    if (image_memory == VK_NULL_HANDLE)
        error_log("Depth resources destruction failed! The depth image memory provided (" + force_string(image_memory) + ") is not valid!");
    else
    {
        vkFreeMemory(logical_device, image_memory, nullptr);
        image_memory = VK_NULL_HANDLE;
    }

    if (image_view == VK_NULL_HANDLE)
        error_log("Depth resources destruction failed! The depth image view provided (" + force_string(image_view) + ") is not valid!");
    else
    {
        vkDestroyImageView(logical_device, image_view, nullptr);
        image_view = VK_NULL_HANDLE;
    }

    log("Depth resources destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Vulkan::Depth::depth_resources_handler::depth_resources_handler
(
    const VkCommandPool &command_pool,
    const VkExtent2D &extent,
    const VkQueue &graphics_queue,
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkSampleCountFlagBits &samples_count
)
    : logical_device(logical_device)
{
    depth_resources = Vulkan::Depth::create_depth_resources(command_pool, extent, graphics_queue, logical_device, physical_device, samples_count);
}

Vulkan::Depth::depth_resources_handler::~depth_resources_handler()
{
    Vulkan::Depth::destroy_depth_resources(depth_resources, logical_device);
}

DepthResources Vulkan::Depth::depth_resources_handler::get() const
{
    return depth_resources;
}
