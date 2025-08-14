#include "buffers.hpp"

#include "loader.hpp"
#include "../buffers/manager.hpp"
#include "../../logs/terminal.hpp"
#include "../../tools/text_format.hpp"

#include <vector>
#include <string>
#include <cstring>
#include <utility>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a buffer for each texture image.
std::vector<Buffer> create_vulkan_texture_image_buffers
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const std::vector<TextureImageInfo> &texture_image_info
)
{
    log("Creating " + std::to_string(texture_image_info.size()) + " texture image buffers..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Texture image buffers creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (!physical_device || physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Texture image buffers creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (texture_image_info.size() < 1)
    {
        fatal_error_log("Texture image buffers creation failed! No texture info were provided!");
    }

    std::vector<Buffer> texture_image_buffers;
    int i = 0;

    // Create a buffer for each texture image.
    for (const TextureImageInfo &info : texture_image_info)
    {
        i++;

        // Retrieve data from the texture image info.
        int image_width = info.width;
        int image_height = info.height;
        VkDeviceSize image_size = info.size;
        stbi_uc* pixels = info.pixels;

        if (!image_width || image_width < 1)
        {
            fatal_error_log("Texture image buffer #" + std::to_string(i) + "/" + std::to_string(texture_image_info.size()) + " creation failed! The image width provided (" + std::to_string(image_width) + ") is not valid!");
        }

        if (!image_height || image_height < 1)
        {
            fatal_error_log("Texture image buffer #" + std::to_string(i) + "/" + std::to_string(texture_image_info.size()) + " creation failed! The image height provided (" + std::to_string(image_height) + ") is not valid!");
        }

        if (!image_size || image_size < 1)
        {
            fatal_error_log("Texture image buffer #" + std::to_string(i) + "/" + std::to_string(texture_image_info.size()) + " creation failed! The image size provided (" + std::to_string(image_size) + ") is not valid!");
        }

        if (!pixels)
        {
            fatal_error_log("Texture image buffer #" + std::to_string(i) + "/" + std::to_string(texture_image_info.size()) + " creation failed! The image provided (" + force_string(pixels) + ") is not valid!");
        }

        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory buffer_memory = VK_NULL_HANDLE;

        create_vulkan_buffer(logical_device, physical_device, image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, buffer, buffer_memory);
        void* data;

        vkMapMemory(logical_device, buffer_memory, 0, image_size, 0, &data); // Map the buffer memory in the app address space.
        memcpy(data, pixels, static_cast<size_t>(image_size));               // Copy the data from CPU memory to GPU memory to enhance performances.
        vkUnmapMemory(logical_device, buffer_memory);                        // Unmap the memory once we finished.

        // Format the buffer data.
        Buffer buffer_data
        {
            buffer,
            buffer_memory
        };

        texture_image_buffers.emplace_back(buffer_data); // Register the buffer.
        log("- Texture image buffer #" + std::to_string(i) + "/" + std::to_string(texture_image_info.size()) + " (" + force_string(buffer) + ") created successfully!");
    }

    log(std::to_string(texture_image_info.size()) + " texture image buffers created successfully!");
    return texture_image_buffers;
}

// Destroy some texture image buffers.
void destroy_vulkan_texture_image_buffers
(
    const VkDevice &logical_device,
    std::vector<Buffer> &texture_image_buffers
)
{
    log("Destroying " + std::to_string(texture_image_buffers.size()) + " image texture buffers..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        error_log("Image texture buffers destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (texture_image_buffers.size() < 1)
    {
        error_log("Image texture buffers destruction failed! No image texture info were provided!");
        return;
    }

    int failed = 0;
    int i = 0;

    // Destroy each texture image buffer in the list.
    for (Buffer &buffer_data : texture_image_buffers)
    {
        i++;

        // Retrieve buffer data from the pair.
        VkBuffer buffer = buffer_data.buffer;
        VkDeviceMemory buffer_memory = buffer_data.buffer_memory;

        if (!buffer || buffer == VK_NULL_HANDLE)
        {
            error_log("- Warning: Failed to destroy the image texture buffer #" + std::to_string(i) + "/" + std::to_string(texture_image_buffers.size()) + "! The buffer provided (" + force_string(buffer) + ") is not valid!");
            failed++;
            continue;
        }

        if (!buffer_memory || buffer_memory == VK_NULL_HANDLE)
        {
            error_log("- Warning: Failed to destroy the image texture buffer #" + std::to_string(i) + "/" + std::to_string(texture_image_buffers.size()) + "! The buffer memory provided (" + force_string(buffer_memory) + ") is not valid!");
            failed++;
            continue;
        }

        destroy_vulkan_buffer(logical_device, buffer, buffer_memory);
        log("- Image texture buffer #" + std::to_string(i) + "/" + std::to_string(texture_image_buffers.size()) + " destroyed successfully!");
    }

    if (failed > 0) error_log("Warning: " + std::to_string(failed) + " image texture buffers failed to destroy! This might lead to some memory leaks and memory overload.");
    log(std::to_string(texture_image_buffers.size() - failed) + "/" + std::to_string(texture_image_buffers.size()) + " image texture buffers destroyed successfully!");

    // Free the list.
    texture_image_buffers.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_TextureImageBuffers::Vulkan_TextureImageBuffers
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const std::vector<TextureImageInfo> &texture_image_info
) : logical_device(logical_device)
{
    texture_image_buffers = create_vulkan_texture_image_buffers(logical_device, physical_device, texture_image_info);
}

// Destructor.
Vulkan_TextureImageBuffers::~Vulkan_TextureImageBuffers()
{
    destroy_vulkan_texture_image_buffers(logical_device, texture_image_buffers);
}

std::vector<Buffer> Vulkan_TextureImageBuffers::get() const
{
    return texture_image_buffers;
}
