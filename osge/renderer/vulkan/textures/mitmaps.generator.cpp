#include "mitmaps.generator.hpp"

#include "../commands/command.buffer.handler.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <string>

// Generate mitmaps for an image.
void generate_mipmaps
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkImage &image,
    const VkFormat &image_format,
    const int &width,
    const int &height,
    const uint32_t &mip_levels
)
{
    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Mitmaps generation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Mitmaps generation failed! The physical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (command_pool == VK_NULL_HANDLE)
    {
        fatal_error_log("Mitmaps generation failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");
    }

    if (graphics_queue == VK_NULL_HANDLE)
    {
        fatal_error_log("Mitmaps generation failed! The graphics queue provided (" + force_string(graphics_queue) + ") is not valid!");
    }

    if (image == VK_NULL_HANDLE)
    {
        fatal_error_log("Mitmaps generation failed! The image provided (" + force_string(image) + ") is not valid!");
    }

    if (width < 1)
    {
        fatal_error_log("Mitmaps generation failed! The width provided (" + std::to_string(width) + ") is not valid!");
    }

    if (height < 1)
    {
        fatal_error_log("Mitmaps generation failed! The height provided (" + std::to_string(height) + ") is not valid!");
    }

    if (mip_levels < 1)
    {
        fatal_error_log("Mitmaps generation failed! The mip levels count provided (" + std::to_string(mip_levels) + ") is not valid!");
    }

    VkFormatProperties format_properties;
    vkGetPhysicalDeviceFormatProperties(physical_device, image_format, &format_properties);

    // Verify linear filtering support.
    if (!(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
    {
        fatal_error_log("Mitmaps generation failed! The texture image format doesn't support linear blitting!");
    }

    VkCommandBuffer command_buffer = begin_one_time_vulkan_command_buffer(logical_device, command_pool);

    // Used for layout transitions between mip levels.
    VkImageMemoryBarrier barrier
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = image,
        .subresourceRange
        {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        }
    };

    int mip_width = width;
    int mip_height = height;

    for (int i = 1; i < mip_levels; i++)
    {
        // Transition previous mip level.
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier
        (
            command_buffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            0,
            0,
            nullptr,
            0,
            nullptr,
            1,
            &barrier
        );

        // Blit operation between mip levels.
        VkImageBlit blit
        {
            .srcSubresource
            {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel = static_cast<uint32_t>(i - 1),
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
            .dstSubresource
            {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel = static_cast<uint32_t>(i),
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };

        // Source mip.
        blit.srcOffsets[0] = { 0, 0, 0 };
        blit.srcOffsets[1] = { mip_width, mip_height, 1 };

        // Destination mip.
        blit.dstOffsets[0] = { 0, 0, 0 };
        blit.dstOffsets[1] = { mip_width > 1 ? mip_width / 2 : 1, mip_height > 1 ? mip_height / 2 : 1, 1 };

        vkCmdBlitImage
        (
            command_buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &blit,
            VK_FILTER_LINEAR
        );

        // Transition previous mip level.
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier
        (
            command_buffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            0,
            0,
            nullptr,
            0,
            nullptr,
            1,
            &barrier
        );

        // Reduce resolution for the next mip level.
        if (mip_width > 1) mip_width /= 2;
        if (mip_height > 1) mip_height /= 2;
    }

    // Transition the last mip level.
    barrier.subresourceRange.baseMipLevel = mip_levels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier
    (
        command_buffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0,
        0,
        nullptr,
        0,
        nullptr,
        1,
        &barrier
    );

    end_command_buffer(logical_device, command_pool, graphics_queue, command_buffer);
}
