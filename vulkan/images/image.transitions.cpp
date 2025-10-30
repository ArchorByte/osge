#include "image.transitions.hpp"

#include "../commands/command.buffer.handler.hpp"
#include "../depth/depth.formats.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>

// Make the transition layout for an image.
void transition_image_layout
(
    const VkDevice &logical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkImage &image,
    const VkFormat &format,
    const VkImageLayout &old_layout,
    const VkImageLayout &new_layout
)
{
    log(" > Transitioning an image layout from " + std::to_string(old_layout) + " to " + std::to_string(new_layout) + "..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Image layout transition failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (command_pool == VK_NULL_HANDLE)
    {
        fatal_error_log("Image layout transition failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");
    }

    if (graphics_queue == VK_NULL_HANDLE)
    {
        fatal_error_log("Image layout transition failed! The graphics queue provided (" + force_string(graphics_queue) + ") is not valid!");
    }

    if (image == VK_NULL_HANDLE)
    {
        fatal_error_log("Image layout transition failed! The image provided (" + force_string(image) + ") is not valid!");
    }

    VkCommandBuffer command_buffer = begin_one_time_vulkan_command_buffer(logical_device, command_pool);
    VkPipelineStageFlags source_stage;
    VkPipelineStageFlags destination_stage;

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
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        }
    };

    if (new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        if (has_stencil_component(format))
        {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    }
    else barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destination_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else fatal_error_log("Image layout transition failed! The layout transition requested is not supported!");

    vkCmdPipelineBarrier(command_buffer, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    end_command_buffer(logical_device, command_pool, graphics_queue, command_buffer);

    log(" > Image layout transition done successfully!");
}
