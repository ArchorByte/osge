#include "vulkan.buffers.hpp"

#include "libraries/stb/stb_image.h"
#include "osge/utils/utils.hpp"

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
        1) Verify function parameters.
        2) Create a buffer for each texture image.
        3) Insert the texture image data into the buffers.

    Parameters:
        - logical_device     / VkDevice                 / Logical device of the Vulkan instance.
        - physical_device    / VkPhysicalDevice         / Physical device used to run this Vulkan.
        - texture_image_info / vector<TextureImageInfo> / All necessary information about the texture images.

    Returns:
        A vector list containing all created texture image buffers.
*/
std::vector<Buffer> Buffers::create_texture_image_buffers
(
    const VkDevice                      &logical_device,
    const VkPhysicalDevice              &physical_device,
    const std::vector<TextureImageInfo> &texture_image_info
)
{
    Utils::Logs::log("Creating " + std::to_string(texture_image_info.size()) + " texture image buffers.. ", false);

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid!");

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical device invalid.");

    if (texture_image_info.size() < 1)
        Utils::Logs::crash_log("Failed! No texture image info provided.");

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
            Utils::Logs::crash_log("Failed! Image width invalid -> " + std::to_string(image_width) + " (#" + std::to_string(i) + ").");

        if (image_height < 1)
            Utils::Logs::crash_log("Failed! Image height invalid -> " + std::to_string(image_height) + " (#" + std::to_string(i) + ").");

        if (image_size < 1)
            Utils::Logs::crash_log("Failed! Image size invalid -> " + std::to_string(image_size) + " (#" + std::to_string(i) + ").");

        if (!pixels)
            Utils::Logs::crash_log("Failed! Image invalid -> " + Utils::Text::get_memory_address(pixels) + " (#" + std::to_string(i) + ").");

        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory buffer_memory = VK_NULL_HANDLE;

        Buffers::create_buffer(buffer, buffer_memory, image_size, logical_device, physical_device, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        void* data;

        vkMapMemory(logical_device, buffer_memory, 0, image_size, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(image_size));
        vkUnmapMemory(logical_device, buffer_memory);

        const Buffer buffer_data = { buffer, buffer_memory };
        texture_image_buffers.emplace_back(buffer_data);
    }

    Utils::Logs::log("Done!", true);
    return texture_image_buffers;
}



/*
    Destroy some texture image buffers.

    Tasks:
        1) Verify function parameters.
        2) Destroy all valid texture image buffers.
        3) Set objects to null.

    Parameters:
        - logical_device        / VkDevice       / Logical device of the Vulkan instance.
        - texture_image_buffers / vector<Buffer> / Texture image buffers to destroy.

    Returns:
        No object returned.
*/
void Buffers::destroy_texture_image_buffers
(
    const VkDevice      &logical_device,+
    std::vector<Buffer> &texture_image_buffers
)
{
    Utils::Logs::log("Destroying " + std::to_string(texture_image_buffers.size()) + " image texture buffers.. ", false);

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! The logical device invalid.", true);
        return;
    }

    if (texture_image_buffers.size() < 1)
    {
        Utils::Logs::log("Done!", true);
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
            failed++;
            continue;
        }

        if (buffer_memory == VK_NULL_HANDLE)
        {
            failed++;
            continue;
        }

        Buffers::destroy_buffer(buffer, buffer_memory, logical_device);
        buffer_data.buffer = VK_NULL_HANDLE;
        buffer_data.buffer_memory = VK_NULL_HANDLE;
    }

    if (failed > 0)
        Utils::Logs::log("Done! Warning: " + std::to_string(failed) + " destructions failed.", true);
    else
        Utils::Logs::log("Done!", true);

    texture_image_buffers.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Buffers::texture_image_buffers_handler::texture_image_buffers_handler
(
    const VkDevice                      &logical_device,
    const VkPhysicalDevice              &physical_device,
    const std::vector<TextureImageInfo> &texture_image_info
)
    : logical_device(logical_device)
{
    texture_image_buffers = Buffers::create_texture_image_buffers(logical_device, physical_device, texture_image_info);
}

Buffers::texture_image_buffers_handler::~texture_image_buffers_handler()
{
    Buffers::destroy_texture_image_buffers(logical_device, texture_image_buffers);
}

std::vector<Buffer> Buffers::texture_image_buffers_handler::get() const
{
    return texture_image_buffers;
}
