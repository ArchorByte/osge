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
    Utils::Logs::log("Copying data from the " + Utils::Text::get_memory_address(source_buffer) + " buffer to the " + Utils::Text::get_memory_address(destination_buffer) + " buffer..");

    if (buffer_size < 1)
        Utils::Logs::crash_error_log("Buffer copy failed! The buffer size provided (" + std::to_string(buffer_size) + ") is not valid!");

    if (command_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Buffer copy failed! The command pool provided (" + Utils::Text::get_memory_address(command_pool) + ") is not valid!");

    if (destination_buffer == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Buffer copy failed! The destination buffer provided (" + Utils::Text::get_memory_address(destination_buffer) + ") is not valid!");

    if (graphics_queue == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Buffer copy failed! The graphics queue provided (" + Utils::Text::get_memory_address(graphics_queue) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Buffer copy failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    if (source_buffer == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Buffer copy failed! The source buffer provided is not valid!");

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

    Utils::Logs::log("Buffer data copied successfully!");
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
    Utils::Logs::log("Copying the " + Utils::Text::get_memory_address(source_buffer) + " buffer data to the " + Utils::Text::get_memory_address(texture_image) + " texture image..");

    if (command_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Buffer copy failed! The command pool provided (" + Utils::Text::get_memory_address(command_pool) + ") is not valid!");

    if (graphics_queue == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Buffer copy failed! The graphics queue provided (" + Utils::Text::get_memory_address(graphics_queue) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Buffer copy failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    if (source_buffer == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Buffer copy failed! The source buffer provided (" + Utils::Text::get_memory_address(source_buffer) + ") is not valid!");

    if (texture_image == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Buffer copy failed! The destination texture image provided (" + Utils::Text::get_memory_address(texture_image) + ") is not valid!");

    if (texture_image_info.width < 1)
        Utils::Logs::crash_error_log("Buffer copy failed! The texture image width provided (" + std::to_string(texture_image_info.width) + ") is not valid!");

    if (texture_image_info.height < 1)
        Utils::Logs::crash_error_log("Buffer copy failed! The texture image height provided (" + std::to_string(texture_image_info.height) + ") is not valid!");

    VkCommandBuffer command_buffer = Buffers::create_one_time_command_buffer(command_pool, logical_device);

    const uint32_t image_width = static_cast<uint32_t>(texture_image_info.width);
    const uint32_t image_height = static_cast<uint32_t>(texture_image_info.height);

    /*
        bufferOffset         / Defines the starting position for reading.
        bufferRowLength      / Defines how many texels per row. Here, we set "0" to force Vulkan to use the image width.
        bufferImageHeight    / Defines the "height" of the buffer memory. Here, we set "0" to force Vulkan to use the image height.
        imageSubresource     / Defines which part of the image we want to copy.
            - aspectMask     / Defines which aspect of the image we want to copy.
            - mipLevel       / Defines which mip level we want to copy.
            - baseArrayLayer / Defines the starting layer.
            - layerCount     / Defines how many layers we want to copy.
        imageOffset          / Defines the starting position for writing.
        imageExtent          / Defines the image resolution and depth.
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

    Utils::Logs::log("Buffer data copied to texture image successfully!");
}
