#include "vulkan.textures.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/renderer/vulkan/vulkan.renderer.hpp"
#include "osge/utils/utils.hpp"

#include <string>

/*
    Generate mipmaps for a Vulkan image.

    Tasks:
        1) 
*/
void generate_mipmaps
(
    const VkCommandPool    &command_pool,
    const VkQueue          &graphics_queue,
    const int              &height,
    const VkImage          &image,
    const VkFormat         &image_format,
    const VkDevice         &logical_device,
    const uint32_t         &mip_levels,
    const VkPhysicalDevice &physical_device,
    const int              &width
)
{
    Utils::Logs::log("Generating mipmaps.. ", false);

    if (command_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Command pool invalid.");

    if (graphics_queue == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Graphics queue invalid.");

    if (height < 1)
        Utils::Logs::crash_log("Failed! Image height invalid -> " + std::to_string(height) + ".");

    if (image == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Image invalid.");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (mip_levels < 1)
        Utils::Logs::crash_log("Failed! Mip levels count invalid -> " + std::to_string(mip_levels) + ".");

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical device invalid.");

    if (width < 1)
        Utils::Logs::crash_log("Failed! Image width invalid -> " + std::to_string(width) + ".");

    VkFormatProperties format_properties;
    vkGetPhysicalDeviceFormatProperties(physical_device, image_format, &format_properties);

    if (!(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
        Utils::Logs::crash_log("Failed! Image format does not support linear sampling.");

    VkCommandBuffer command_buffer = Vulkan::Buffers::create_one_time_command_buffer(command_pool, logical_device);

    /*
        - sType               / Defines the type of the structure.
        - srcQueueFamilyIndex / 
        - dstQueueFamilyIndex / 
        - image               / 
        - subresourceRange    / 
            - aspectMask      / 
            - levelCount      / 
            - baseArrayLayer  / 
            - layerCount      / 
    */
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
        /*
            
        */
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

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

        vkCmdBlitImage(command_buffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

        // Transition previous mip level.
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

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

    vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    Vulkan::Buffers::destroy_command_buffer(command_buffer, command_pool, graphics_queue, logical_device);
    Utils::Logs::log("Done!", true);
}
