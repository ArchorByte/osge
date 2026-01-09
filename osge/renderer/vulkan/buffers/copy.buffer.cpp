#include "vulkan.buffers.hpp"

#include "../textures/texture.images.loader.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>

/*
    Copy the data of a buffer to another one.

    Tasks:
        1) Verify the parameters.
        2) Create a temporary command buffer to transfer the data.
        3) Select the copy region (what we want to copy, in this case, the whole buffer).
        4) Copy the data from the source buffer to the destination one.
        5) End the command buffer.

    Parameters:
        - buffer_size        / VkDeviceSize  / Size of the source buffer.
        - command_pool       / VkCommandPool / Command pool of the Vulkan instance.
        - destination_buffer / VkBuffer      / Buffer in which we will put the source buffer data.
        - graphics_queue     / VkQueue       / Graphics queue of the Vulkan instance.
        - logical_device     / VkDevice      / Logical device of the Vulkan instance.
        - source_buffer      / VkBuffer      / Source buffer that we will copy the data from.

    Returns:
        No object returned.
*/
void Vulkan::Buffers::copy_buffer_data
(
    const VkDeviceSize &buffer_size,
    const VkCommandPool &command_pool,
    const VkBuffer &destination_buffer,
    const VkQueue &graphics_queue,
    const VkDevice &logical_device,
    const VkBuffer &source_buffer
)
{
    log("Copying data from the " + force_string(source_buffer) + " buffer to the " + force_string(destination_buffer) + " buffer..");

    if (buffer_size < 1)
        fatal_error_log("Buffer copy failed! The buffer size provided (" + std::to_string(buffer_size) + ") is not valid!");

    if (command_pool == VK_NULL_HANDLE)
        fatal_error_log("Buffer copy failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");

    if (destination_buffer == VK_NULL_HANDLE)
        fatal_error_log("Buffer copy failed! The destination buffer provided (" + force_string(destination_buffer) + ") is not valid!");

    if (graphics_queue == VK_NULL_HANDLE)
        fatal_error_log("Buffer copy failed! The graphics queue provided (" + force_string(graphics_queue) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        fatal_error_log("Buffer copy failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");

    if (source_buffer == VK_NULL_HANDLE)
        fatal_error_log("Buffer copy failed! The source buffer provided is not valid!");

    VkCommandBuffer command_buffer = Vulkan::Buffers::create_one_time_command_buffer(command_pool, logical_device);

    VkBufferCopy copy_region
    {
        .srcOffset = 0,
        .dstOffset = 0,
        .size = buffer_size
    };

    vkCmdCopyBuffer(command_buffer, source_buffer, destination_buffer, 1, &copy_region);
    Vulkan::Buffers::destroy_command_buffer(command_buffer, command_pool, graphics_queue, logical_device);

    log("Buffer data copied successfully!");
}



/*
    Copy the data of a buffer to a texture image.

    Tasks:
        1) Verify the parameters.
        2) Create a temporary command buffer to transfer the data.
        3) Select the copy region (what we want to copy, in this case, the whole buffer).
        4) Copy the data from the source buffer to the texture image.
        5) End the command buffer.

    Parameters:
        - command_pool       / VkCommandPool    / Command pool of the Vulkan instance.
        - graphics_queue     / VkQueue          / Graphics queue of the Vulkan instance.
        - logical_device     / VkDevice         / Logical device of the Vulkan instance.
        - source_buffer      / VkBuffer         / Source buffer that we will copy the data from.
        - texture_image      / VkImage          / Targeted texture image.
        - texture_image_info / TextureImageInfo / All necessary information about the texture image.

    Returns:
        No object returned.
*/
void Vulkan::Buffers::copy_buffer_to_texture_image
(
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkDevice &logical_device,
    const VkBuffer &source_buffer,
    const VkImage &texture_image,
    const TextureImageInfo &texture_image_info
)
{
    log(" > Copying the " + force_string(source_buffer) + " buffer data to the " + force_string(texture_image) + " texture image..");

    if (command_pool == VK_NULL_HANDLE)
        fatal_error_log("Buffer copy failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");

    if (graphics_queue == VK_NULL_HANDLE)
        fatal_error_log("Buffer copy failed! The graphics queue provided (" + force_string(graphics_queue) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        fatal_error_log("Buffer copy failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");

    if (source_buffer == VK_NULL_HANDLE)
        fatal_error_log("Buffer copy failed! The source buffer provided (" + force_string(source_buffer) + ") is not valid!");

    if (texture_image == VK_NULL_HANDLE)
        fatal_error_log("Buffer copy failed! The destination texture image provided (" + force_string(texture_image) + ") is not valid!");

    if (texture_image_info.width < 1)
        fatal_error_log("Buffer copy failed! The texture image width provided (" + std::to_string(texture_image_info.width) + ") is not valid!");

    if (texture_image_info.height < 1)
        fatal_error_log("Buffer copy failed! The texture image height provided (" + std::to_string(texture_image_info.height) + ") is not valid!");

    VkCommandBuffer command_buffer = Vulkan::Buffers::create_one_time_command_buffer(command_pool, logical_device);

    const uint32_t image_width = static_cast<uint32_t>(texture_image_info.width);
    const uint32_t image_height = static_cast<uint32_t>(texture_image_info.height);

    VkBufferImageCopy copy_region
    {
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource =
        {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, // Target the color aspect.
            .mipLevel = 0,
            .baseArrayLayer = 0, // Target the first layer of the texture image.
            .layerCount = 1
        },
        .imageOffset = { 0, 0, 0 },
        .imageExtent = { image_width, image_height, 1 }
    };

    vkCmdCopyBufferToImage(command_buffer, source_buffer, texture_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_region);
    Vulkan::Buffers::destroy_command_buffer(command_buffer, command_pool, graphics_queue, logical_device);

    log(" > Buffer data copied to texture image successfully!");
}
