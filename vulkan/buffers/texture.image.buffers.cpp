#include "vulkan.buffers.hpp"

#include "texture.images.loader.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <cstring>
#include <string>
#include <utility>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a buffer for each texture image.
    Note: You should use the pre-made class to handle the texture image buffers rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the parameters.

    Parameters:
        - logical_device     / VkDevice                 / Logical device of the Vulkan instance.
        - physical_device    / VkPhysicalDevice         / Physical device used to run Vulkan.
        - texture_image_info / vector<TextureImageInfo> / Information of the targeted texture images.

    Returns:
        A vector list containing all created texture image buffers.
*/
std::vector<Buffer> Vulkan::Buffers::create_texture_image_buffers
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const std::vector<TextureImageInfo> &texture_image_info
)
{
    log("Creating " + std::to_string(texture_image_info.size()) + " texture image buffers..");

    if (logical_device == VK_NULL_HANDLE)
        fatal_error_log("Texture image buffers creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");

    if (physical_device == VK_NULL_HANDLE)
        fatal_error_log("Texture image buffers creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");

    if (texture_image_info.size() < 1)
        fatal_error_log("Texture image buffers creation failed! No texture image info provided!");

    std::vector<Buffer> texture_image_buffers;
    int i = 0;

    for (const TextureImageInfo &image_info : texture_image_info)
    {
        i++;

        const int image_width = image_info.width;
        const int image_height = image_info.height;
        const VkDeviceSize image_size = image_info.size;
        const stbi_uc* pixels = image_info.pixels;

        if (image_width < 1)
            fatal_error_log("Texture image buffer #" + std::to_string(i) + "/" + std::to_string(texture_image_info.size()) + " creation failed! The image width provided (" + std::to_string(image_width) + ") is not valid!");

        if (image_height < 1)
            fatal_error_log("Texture image buffer #" + std::to_string(i) + "/" + std::to_string(texture_image_info.size()) + " creation failed! The image height provided (" + std::to_string(image_height) + ") is not valid!");

        if (image_size < 1)
            fatal_error_log("Texture image buffer #" + std::to_string(i) + "/" + std::to_string(texture_image_info.size()) + " creation failed! The image size provided (" + std::to_string(image_size) + ") is not valid!");

        if (!pixels)
            fatal_error_log("Texture image buffer #" + std::to_string(i) + "/" + std::to_string(texture_image_info.size()) + " creation failed! The image provided (" + force_string(pixels) + ") is not valid!");

        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory buffer_memory = VK_NULL_HANDLE;

        Vulkan::Buffers::create_buffer(buffer, buffer_memory, image_size, logical_device, physical_device, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        void* data;

        vkMapMemory(logical_device, buffer_memory, 0, image_size, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(image_size));
        vkUnmapMemory(logical_device, buffer_memory);

        const Buffer buffer_data = { buffer, buffer_memory };
        texture_image_buffers.emplace_back(buffer_data);

        log("- Texture image buffer #" + std::to_string(i) + "/" + std::to_string(texture_image_info.size()) + " (" + force_string(buffer) + ") created successfully!");
    }

    log(std::to_string(texture_image_info.size()) + " texture image buffers created successfully!");
    return texture_image_buffers;
}



/*
    Cleanly destroy the texture image buffers.

    Tasks:
        1) Verify the parameters.
        2) Destroy all valid texture image buffers.

    Parameters:
        - logical_device        / VkDevice       / Logical device of the Vulkan instance.
        - texture_image_buffers / vector<Buffer> / Texture image buffers to destroy.

    Returns:
        No object returned.
*/
void Vulkan::Buffers::destroy_texture_image_buffers
(
    const VkDevice &logical_device,
    std::vector<Buffer> &texture_image_buffers
)
{
    log("Destroying " + std::to_string(texture_image_buffers.size()) + " image texture buffers..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Image texture buffers destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (texture_image_buffers.size() < 1)
    {
        error_log("Image texture buffers destruction failed! No image texture info provided!");
        return;
    }

    int failed = 0;
    int i = 0;

    for (Buffer &buffer_data : texture_image_buffers)
    {
        i++;

        VkBuffer buffer = buffer_data.buffer;
        VkDeviceMemory buffer_memory = buffer_data.buffer_memory;

        if (buffer == VK_NULL_HANDLE)
        {
            error_log("- Failed to destroy the texture image buffer #" + std::to_string(i) + "/" + std::to_string(texture_image_buffers.size()) + "! The buffer provided (" + force_string(buffer) + ") is not valid!");
            failed++;
            continue;
        }

        if (buffer_memory == VK_NULL_HANDLE)
        {
            error_log("- Failed to destroy the texture image buffer #" + std::to_string(i) + "/" + std::to_string(texture_image_buffers.size()) + "! The buffer memory provided (" + force_string(buffer_memory) + ") is not valid!");
            failed++;
            continue;
        }

        Vulkan::Buffers::destroy_buffer(buffer, buffer_memory, logical_device);
        log("- Texture image buffer #" + std::to_string(i) + "/" + std::to_string(texture_image_buffers.size()) + " destroyed successfully!");
    }

    if (failed > 0)
        error_log("Warning: " + std::to_string(failed) + " texture image buffers failed to destroy! This might lead to some memory leaks and memory overload.");

    log(std::to_string(texture_image_buffers.size() - failed) + "/" + std::to_string(texture_image_buffers.size()) + " texture image buffers destroyed successfully!");
    texture_image_buffers.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Vulkan::Buffers::texture_image_buffers_handler::texture_image_buffers_handler
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const std::vector<TextureImageInfo> &texture_image_info
)
    : logical_device(logical_device)
{
    texture_image_buffers = Vulkan::Buffers::create_texture_image_buffers(logical_device, physical_device, texture_image_info);
}

Vulkan::Buffers::texture_image_buffers_handler::~texture_image_buffers_handler()
{
    Vulkan::Buffers::destroy_texture_image_buffers(logical_device, texture_image_buffers);
}

std::vector<Buffer> Vulkan::Buffers::texture_image_buffers_handler::get() const
{
    return texture_image_buffers;
}
