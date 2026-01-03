#include "vulkan.images.hpp"

#include "../buffers/vulkan.buffers.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>

/*
    Transition an image from a layout to another.

    Tasks:
        1) Verify the parameters.

    Parameters:
        - command_pool   / VkCommandPool / Command pool of the Vulkan instance.
        - format         / VkFormat      / Format of the image.
        - graphics_queue / VkQueue       / Graphics queue of the Vulkan instance.
        - image          / VkImage       / Targeted image.
        - logical_device / VkDevice      / Logical device of the Vulkan instance.
        - mip_levels     / uint32_t      / Mip levels used for LOD.
        - new_layout     / VkImageLayout / Layout we want to transition to.
        - old_layout     / VkImageLayout / Layout we want to transition from.

    Returns:
        No object returned.
*/
void Vulkan::Images::transition_image_layout
(
    const VkCommandPool &command_pool,
    const VkFormat &format,
    const VkQueue &graphics_queue,
    const VkImage &image,
    const VkDevice &logical_device,
    const uint32_t &mip_levels,
    const VkImageLayout &new_layout,
    const VkImageLayout &old_layout
)
{
    log("Transitioning an image layout from " + std::to_string(old_layout) + " to " + std::to_string(new_layout) + "..");

    if (command_pool == VK_NULL_HANDLE)
        fatal_error_log("Image layout transition failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");

    if (graphics_queue == VK_NULL_HANDLE)
        fatal_error_log("Image layout transition failed! The graphics queue provided (" + force_string(graphics_queue) + ") is not valid!");

    if (image == VK_NULL_HANDLE)
        fatal_error_log("Image layout transition failed! The image provided (" + force_string(image) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        fatal_error_log("Image layout transition failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");

    if (mip_levels < 1)
        fatal_error_log("Image layout transition failed! The mip levels count provided (" + std::to_string(mip_levels) + ") is not valid!");

    VkCommandBuffer command_buffer = Vulkan::Buffers::create_one_time_command_buffer(command_pool, logical_device);
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
            .levelCount = mip_levels,
            .baseArrayLayer = 0,
            .layerCount = 1
        }
    };

    if (new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT)
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
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
    Vulkan::Buffers::destroy_command_buffer(command_buffer, command_pool, graphics_queue, logical_device);

    log("Image layout transition done successfully!");
}
