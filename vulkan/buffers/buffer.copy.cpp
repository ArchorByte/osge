#include "buffer.copy.hpp"

#include "../commands/command.buffer.handler.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"
#include "../textures/texture.images.loader.hpp"

#include <vulkan/vulkan.h>

// Copy the data from a source buffer to a destination buffer.
void copy_vulkan_buffer_data
(
    const VkDevice &logical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkBuffer &source_buffer,
    const VkBuffer &destination_buffer,
    const VkDeviceSize &buffer_size
)
{
    log(" > Copying data from the " + force_string(source_buffer) + " buffer to " + force_string(destination_buffer) + " buffer..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Buffer copy failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (!command_pool || command_pool == VK_NULL_HANDLE)
    {
        fatal_error_log("Buffer copy failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");
    }

    if (!graphics_queue || graphics_queue == VK_NULL_HANDLE)
    {
        fatal_error_log("Buffer copy failed! The graphics queue provided (" + force_string(graphics_queue) + ") is not valid!");
    }

    if (!source_buffer || source_buffer == VK_NULL_HANDLE)
    {
        fatal_error_log("Buffer copy failed! The source buffer provided is not valid!");
    }

    if (!destination_buffer || destination_buffer == VK_NULL_HANDLE)
    {
        fatal_error_log("Buffer copy failed! The destination buffer provided is not valid!");
    }

    if (buffer_size < 1)
    {
        fatal_error_log("Buffer copy failed! The buffer size provided (" + std::to_string(buffer_size) + ") is not valid!");
    }

    VkCommandBuffer command_buffer = begin_one_time_vulkan_command_buffer(logical_device, command_pool);

    // Region in the command buffer to copy.
    VkBufferCopy copy_region {};
    copy_region.srcOffset = 0;      // Start to read data from the begining of the source buffer.
    copy_region.dstOffset = 0;      // Start to write the same data from the begining of the destination buffer.
    copy_region.size = buffer_size; // Amount of bytes to copy.

    // Make the copy of the buffer.
    vkCmdCopyBuffer(command_buffer, source_buffer, destination_buffer, 1, &copy_region);

    end_command_buffer(logical_device, command_pool, graphics_queue, command_buffer);
    log(" > Buffer data copied successfully!");
}

// Copy the data of a buffer to a texture image.
void copy_vulkan_buffer_to_texture_image
(
    const VkDevice &logical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkBuffer &buffer,
    const VkImage &image,
    const TextureImageInfo &image_info
)
{
    log(" > Copying the " + force_string(buffer) + " buffer data to the " + force_string(image) + " texture images..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Buffer copy failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (!command_pool || command_pool == VK_NULL_HANDLE)
    {
        fatal_error_log("Buffer copy failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");
    }

    if (!graphics_queue || graphics_queue == VK_NULL_HANDLE)
    {
        fatal_error_log("Buffer copy failed! The graphics queue provided (" + force_string(graphics_queue) + ") is not valid!");
    }

    if (!buffer || buffer == VK_NULL_HANDLE)
    {
        fatal_error_log("Buffer copy failed! The source buffer provided (" + force_string(buffer) + ") is not valid!");
    }

    if (!image || image == VK_NULL_HANDLE)
    {
        fatal_error_log("Buffer copy failed! The destination texture image provided (" + force_string(image) + ") is not valid!");
    }

    if (image_info.width < 1)
    {
        fatal_error_log("Buffer copy failed! The texture image width provided (" + std::to_string(image_info.width) + ") is not valid!");
    }

    if (image_info.height < 1)
    {
        fatal_error_log("Buffer copy failed! The texture image height provided (" + std::to_string(image_info.height) + ") is not valid!");
    }

    VkCommandBuffer command_buffer = begin_one_time_vulkan_command_buffer(logical_device, command_pool);

    // Convert the int data.
    uint32_t width = static_cast<uint32_t>(image_info.width);
    uint32_t height = static_cast<uint32_t>(image_info.height);

    // Info for the buffer to texture image copy.
    VkBufferImageCopy region {};
    region.bufferOffset = 0;                                        // Offset for the copy. Buffer side.
    region.bufferRowLength = 0;                                     // Row length of the buffer.
    region.bufferImageHeight = 0;                                   // Height of the buffer image.
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // This copy is for the color aspect of our texture image.
    region.imageSubresource.mipLevel = 0;                           // Mipmap level to copy.
    region.imageSubresource.baseArrayLayer = 0;                     // Select the first layer of the image to copy.
    region.imageSubresource.layerCount = 1;                         // Amount of layers to copy.
    region.imageOffset = { 0, 0, 0 };                               // Offset for the copy. Texture image side.
    region.imageExtent = { width, height, 1 };                      // Pass the image size: width, height and depth.

    vkCmdCopyBufferToImage(command_buffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region); // Make the copy.
    end_command_buffer(logical_device, command_pool, graphics_queue, command_buffer);                        // End the command buffer once we are done.

    log(" > Buffer data copied to texture image successfully!");
}
