#include "vulkan.textures.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/renderer/vulkan/vulkan.renderer.hpp"
#include "osge/utils/utils.hpp"

#include <string>

/*
    Generate mipmaps for a Vulkan image.

    Tasks:
        1) Verify function parameters.
        2) Verify linear support.
        3) For each mip level:
            - Transition previous mip level to TRANSFER_SRC_OPTIMAL.
            - Transition current mip level to TRANSFER_DST_OPTIMAL.
            - Perform blit from previous level to current level with linear filtering.
            - Transition previous level to SHADER_READ_ONLY_OPTIMAL.
        4) Transition the last mip level.
        5) Free temporary command buffer.

    Parameters:
        - command_pool    / VkCommandPool    / Handles memory allocation of command buffers.
        - format          / VkFormat         / Defines the format of the image.
        - graphics_queue  / VkQueue          / Handles all graphics commands and calls.
        - height          / int              / Defines the initial height of the image.
        - image           / VkImage          / Image targeted by the mip maps generation.
        - logical_device  / VkDevice         / Logical device of the Vulkan instance.
        - mip_levels      / uint32_t         / Mip levels used for Level Of Details (LOD).
        - physical_device / VkPhysicalDevice / Physical device used to run this Vulkan instance.
        - width           / int              / Defines the initial width of the image.

    Returns:
        No object returned.
*/
void Textures::generate_mipmaps
(
    const VkCommandPool    &command_pool,
    const VkFormat         &format,
    const VkQueue          &graphics_queue,
    const int              &height,
    const VkImage          &image,
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
    vkGetPhysicalDeviceFormatProperties(physical_device, format, &format_properties);

    if (!(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
        Utils::Logs::crash_log("Failed! Image format does not support linear sampling.");

    VkCommandBuffer command_buffer = Buffers::create_one_time_command_buffer(command_pool, logical_device);

    /*
        - sType               / Defines the type of the structure.
        - srcQueueFamilyIndex / Index of the source queue to transfer ownership from. Ignored here.
        - dstQueueFamilyIndex / Index of the destination queue to transfer ownership to. Ignored here.
        - image               / Image affected by the mip maps generation.
        - subresourceRange    / Describes the range of the image subresource.
            - aspectMask      / Defines which aspect of the image is affected.
            - levelCount      / Defines the amount of mip map levels we are going to make.
            - baseArrayLayer  / Defines the first array layer.
            - layerCount      / Defines the amount of layers accessible.
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
            - subresourceRange / Describes the range of the image subresource.
                - baseMipLevel / Defines the first mip level.
            - oldLayout        / Defines the layout we transition from.
            - newLayout        / Defines the layout we transition to.
            - srcAccessMask    / Defines the initial aspect mask.
            - dstAccessMask    / Defines the final aspect mask.
        */
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        /*
            - srcSubresource     / Subresource to blit from.
                - aspectMask     / Define which aspects of the image are going to be affected.
                - mipLevel       / Mip level which is going to be affected.
                - baseArrayLayer / Defines the starting layer.
                - layerCount     / Defines the amount of layers affected.
            - dstSubresource     / Subresource to blit to.
                - aspectMask     / Define which aspects of the image are going to be affected.
                - mipLevel       / Mip level which is going to be affected.
                - baseArrayLayer / Defines the starting layer.
                - layerCount     / Defines the amount of layers affected.
        */
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

        /*
            - srcOffsets / Defines the bounds of the source region.
        */
        blit.srcOffsets[0] = { 0, 0, 0 };
        blit.srcOffsets[1] = { mip_width, mip_height, 1 };

        /*
            - dstOffsets / Defines the bounds of the destination region.
        */
        blit.dstOffsets[0] = { 0, 0, 0 };
        blit.dstOffsets[1] = { mip_width > 1 ? mip_width / 2 : 1, mip_height > 1 ? mip_height / 2 : 1, 1 };

        vkCmdBlitImage(command_buffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

        /*
            - oldLayout        / Defines the layout we transition from.
            - newLayout        / Defines the layout we transition to.
            - srcAccessMask    / Defines the initial aspect mask.
            - dstAccessMask    / Defines the final aspect mask.
        */
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        if (mip_width > 1)
            mip_width /= 2;

        if (mip_height > 1)
            mip_height /= 2;
    }

    /*
        - subresourceRange / Describes the range of the image subresource.
            - baseMipLevel / Defines the first mip level.
        - oldLayout        / Defines the layout we transition from.
        - newLayout        / Defines the layout we transition to.
        - srcAccessMask    / Defines the initial aspect mask.
        - dstAccessMask    / Defines the final aspect mask.
    */
    barrier.subresourceRange.baseMipLevel = mip_levels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    Buffers::destroy_command_buffer(command_buffer, command_pool, graphics_queue, logical_device);
    Utils::Logs::log("Done!", true);
}
