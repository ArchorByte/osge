#include "vulkan.images.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"
#include "osge/renderer/vulkan/vulkan.renderer.hpp"

/*
    Transition an image from a layout to another.

    Tasks:
        1) Verify function parameters.
        2) Create a temporary one-time command buffer for transitioning.
        3) Make an image barrier that contains all transition information.
        4) Adjust the barrier depending on what's the old/new layout.
        5) Make the transition and destroy the command buffer.

    Parameters:
        - command_pool   / VkCommandPool / Handles memory allocation of command buffers.
        - format         / VkFormat      / Defines the format of the image targeted by the transition.
        - graphics_queue / VkQueue       / Handles all graphics commands and calls.
        - image          / VkImage       / Image that must be transitioned.
        - logical_device / VkDevice      / Logical device of this Vulkan instance.
        - mip_levels     / uint32_t      / Mip levels used for Level Of Details (LOD).
        - new_layout     / VkImageLayout / Layout we want to transition to.
        - old_layout     / VkImageLayout / Layout we want to transition from.

    Returns:
        No object returned.
*/
void Images::transition_image_layout
(
    const VkCommandPool &command_pool,
    const VkFormat      &format,
    const VkQueue       &graphics_queue,
    const VkImage       &image,
    const VkDevice      &logical_device,
    const uint32_t      &mip_levels,
    const VkImageLayout &new_layout,
    const VkImageLayout &old_layout
)
{
    Utils::Logs::log("Transitioning an image layout from " + std::to_string(old_layout) + " to " + std::to_string(new_layout) + "..");

    if (command_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Image layout transition failed! The command pool provided (" + Utils::Text::get_memory_address(command_pool) + ") is not valid!");

    if (graphics_queue == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Image layout transition failed! The graphics queue provided (" + Utils::Text::get_memory_address(graphics_queue) + ") is not valid!");

    if (image == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Image layout transition failed! The image provided (" + Utils::Text::get_memory_address(image) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Image layout transition failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    if (mip_levels < 1)
        Utils::Logs::crash_error_log("Image layout transition failed! The mip levels count provided (" + std::to_string(mip_levels) + ") is not valid!");

    VkCommandBuffer command_buffer = Vulkan::Buffers::create_one_time_command_buffer(command_pool, logical_device);
    VkPipelineStageFlags source_stage;
    VkPipelineStageFlags destination_stage;

    /*
        - sType               / Defines the type of the structure.
        - oldLayout           / Defines the layout we want to transition from.
        - newLayout           / Defines the layout we want to transition to.
        - srcQueueFamilyIndex / Defines the index of the source queue family.
        - dstQueueFamilyIndex / Defines the index of the destination queue family.
        - image               / Defines which image is going to be transitioned.
        - subresourceRange    / Describes the range of the image subresource.
            - baseMipLevel    / First mip level to include in the transition.
            - levelCount      / Defines the amount of mip levels affected.
            - baseArrayLayer  / First array layer to include.
            - layerCount      / Defines the amount of array layers affected.
    */
    VkImageMemoryBarrier barrier
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .oldLayout = old_layout,
        .newLayout = new_layout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = image,
        .subresourceRange =
        {
            .baseMipLevel = 0,
            .levelCount = mip_levels,
            .baseArrayLayer = 0,
            .layerCount = 1
        }
    };

    if (new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        // barrier.subresourceRange.aspectMask / Defines which aspect of the image is affected. Here, it's the depth bit aspect.
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT)
            // barrier.subresourceRange.aspectMask / Defines which aspect of the image is affected. Here it's the depth stencil aspect.
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    else
    {
        // barrier.subresourceRange.aspectMask / Defines which aspect of the image is affected. Here it's the color bit aspect.
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        /*
            - barrier.srcAccessMask / Defines the access mask we want to transition from.
            - barrier.dstAccessMask / Defines the access mask we want to transition to.
        */
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        /*
            - barrier.srcAccessMask / Defines the access mask we want to transition from.
            - barrier.dstAccessMask / Defines the access mask we want to transition to.
        */
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        /*
            - barrier.srcAccessMask / Defines the access mask we want to transition from.
            - barrier.dstAccessMask / Defines the access mask we want to transition to.
        */
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destination_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else Utils::Logs::crash_error_log("Image layout transition failed! The layout transition requested is not supported!");

    vkCmdPipelineBarrier(command_buffer, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    Vulkan::Buffers::destroy_command_buffer(command_buffer, command_pool, graphics_queue, logical_device);

    Utils::Logs::log("Image layout transition done successfully!");
}
