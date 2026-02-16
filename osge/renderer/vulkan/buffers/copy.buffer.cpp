#include "vulkan.buffers.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include "../textures/texture.images.loader.hpp"

/*
    Copy the data of a buffer to another one.

    Tasks:
        1) Verify function parameters.
        2) Create a temporary command buffer to transfer the data.
        3) Select the copy region (what we want to copy, in this function, the whole buffer).
        4) Copy the data from the source buffer to the destination one.
        5) End the command buffer.

    Parameters:
        - buffer_size        / VkDeviceSize  / Size of the source buffer.
        - command_pool       / VkCommandPool / Handles memory allocation of command buffers.
        - destination_buffer / VkBuffer      / Buffer in which we will put the source buffer data.
        - graphics_queue     / VkQueue       / Handles all graphics commands and calls.
        - logical_device     / VkDevice      / Logical device of the Vulkan instance.
        - source_buffer      / VkBuffer      / Source buffer that we will copy the data from.

    Returns:
        No object returned.
*/
void Buffers::copy_buffer_data
(
    const VkDeviceSize  &buffer_size,
    const VkCommandPool &command_pool,
    const VkBuffer      &destination_buffer,
    const VkQueue       &graphics_queue,
    const VkDevice      &logical_device,
    const VkBuffer      &source_buffer
)
{
    Utils::Logs::log("Copying data from buffer (" + Utils::Text::get_memory_address(source_buffer) + ") to buffer (" + Utils::Text::get_memory_address(destination_buffer) + ").. ", false);

    if (buffer_size < 1)
        Utils::Logs::crash_log("Failed! Buffer size invalid -> " + std::to_string(buffer_size) + ".");

    if (command_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Command pool invalid.");

    if (destination_buffer == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Destination buffer invalid.");

    if (graphics_queue == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Graphics queue invalid.");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (source_buffer == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Source buffer invalid.");

    VkCommandBuffer command_buffer = Buffers::create_one_time_command_buffer(command_pool, logical_device);

    /*
        srcOffset / Defines the starting position for reading.
        dstOffset / Defines the starting position for writing.
        size      / Amount of bytes to copy.
    */
    const VkBufferCopy copy_region
    {
        .srcOffset = 0,
        .dstOffset = 0,
        .size = buffer_size
    };

    vkCmdCopyBuffer(command_buffer, source_buffer, destination_buffer, 1, &copy_region);
    Buffers::destroy_command_buffer(command_buffer, command_pool, graphics_queue, logical_device);

    Utils::Logs::log("Done!", true);
}



/*
    Copy the data of a buffer to a texture image.

    Tasks:
        1) Verify the function parameters.
        2) Create a temporary command buffer to transfer the data.
        3) Select the copy region (what we want to copy, in this function, the whole buffer).
        4) Copy the data from the source buffer to the texture image.
        5) End the command buffer.

    Parameters:
        - command_pool       / VkCommandPool    / Handles memory allocation of the command buffers.
        - graphics_queue     / VkQueue          / Handles all graphics commands and calls.
        - logical_device     / VkDevice         / Logical device of the Vulkan instance.
        - source_buffer      / VkBuffer         / Source buffer that we will copy the data from.
        - texture_image      / VkImage          / Texture image to write the source buffer data into.
        - texture_image_info / TextureImageInfo / All necessary information about the texture image.

    Returns:
        No object returned.
*/
void Buffers::copy_buffer_to_texture_image
(
    const VkCommandPool    &command_pool,
    const VkQueue          &graphics_queue,
    const VkDevice         &logical_device,
    const VkBuffer         &source_buffer,
    const VkImage          &texture_image,
    const TextureImageInfo &texture_image_info
)
{
    Utils::Logs::log("Copying data from buffer (" + Utils::Text::get_memory_address(source_buffer) + ") to texture image (" + Utils::Text::get_memory_address(texture_image) + ").. ", false);

    if (command_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Command pool invalid.");

    if (graphics_queue == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Graphics queue invalid.");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (source_buffer == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Source buffer invalid.");

    if (texture_image == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Destination image invalid.");

    if (texture_image_info.width < 1)
        Utils::Logs::crash_log("Failed! Image width invalid -> " + std::to_string(texture_image_info.width) + ".");

    if (texture_image_info.height < 1)
        Utils::Logs::crash_log("Failed! Image height invalid -> " + std::to_string(texture_image_info.height) + ".");

    VkCommandBuffer command_buffer = Buffers::create_one_time_command_buffer(command_pool, logical_device);

    const uint32_t image_width = static_cast<uint32_t>(texture_image_info.width);
    const uint32_t image_height = static_cast<uint32_t>(texture_image_info.height);

    /*
        - bufferOffset         / Defines the starting position for reading.
        - bufferRowLength      / Defines how many texels per row. Here, we set "0" to force Vulkan to use the image width.
        - bufferImageHeight    / Defines the "height" of the buffer memory. Here, we set "0" to force Vulkan to use the image height.
        - imageSubresource     / Defines which part of the image we want to copy.
            - aspectMask       / Defines which aspect of the image we want to copy.
            - mipLevel         / Defines which mip level we want to copy.
            - baseArrayLayer   / Defines the starting layer.
            - layerCount       / Defines how many layers we want to copy.
        - imageOffset          / Defines the starting position for writing.
        - imageExtent          / Defines the image resolution and depth.
    */
    VkBufferImageCopy copy_region
    {
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource =
        {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = 0,
            .baseArrayLayer = 0,
            .layerCount = 1
        },
        .imageOffset = { 0, 0, 0 },
        .imageExtent = { image_width, image_height, 1 }
    };

    vkCmdCopyBufferToImage(command_buffer, source_buffer, texture_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_region);
    Buffers::destroy_command_buffer(command_buffer, command_pool, graphics_queue, logical_device);

    Utils::Logs::log("Done!", true);
}
