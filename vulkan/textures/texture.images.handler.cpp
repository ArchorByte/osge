#include "texture.images.handler.hpp"

#include "texture.images.loader.hpp"
#include "texture.image.buffers.hpp"
#include "../buffers/buffer.copy.hpp"
#include "../images/image.transitions.hpp"
#include "../images/images.handler.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a texture image for each texture image info.
std::vector<TextureImage> create_vulkan_texture_images
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const std::vector<TextureImageInfo> &texture_image_info,
    Vulkan_TextureImageBuffers &texture_image_buffers
)
{
    log("Creating " + std::to_string(texture_image_info.size()) + " texture images..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Texture images creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Texture images creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (command_pool == VK_NULL_HANDLE)
    {
        fatal_error_log("Texture images creation failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");
    }

    if (graphics_queue == VK_NULL_HANDLE)
    {
        fatal_error_log("Texture images creation failed! The graphics queue provided (" + force_string(graphics_queue) + ") is not valid!");
    }

    if (texture_image_info.size() < 1)
    {
        fatal_error_log("Texture images creation failed! No texture image info were provided!");
    }

    if (texture_image_buffers.get().size() < 1)
    {
        fatal_error_log("Texture images creation failed! No texture image buffers were provided!");
    }

    std::vector<TextureImage> texture_images;
    int i = 0;

    for (const TextureImageInfo &info : texture_image_info)
    {
        i++;

        // Retrieve data from the texture image info.
        const std::string texture_name = info.name;
        const int image_width = info.width;
        const int image_height = info.height;

        if (trim(texture_name).size() < 1)
        {
            fatal_error_log("Texture image #" + std::to_string(i) + "/" + std::to_string(texture_image_info.size()) + " creation failed! The texture name provided (" + texture_name + ") is not valid!");
        }

        if (image_width < 1)
        {
            fatal_error_log("Texture image #" + std::to_string(i) + "/" + std::to_string(texture_image_info.size()) + " creation failed! The image width provided (" + std::to_string(image_width) + ") is not valid!");
        }

        if (image_height < 1)
        {
            fatal_error_log("Texture image #" + std::to_string(i) + "/" + std::to_string(texture_image_info.size()) + " creation failed! The image height provided (" + std::to_string(image_height) + ") is not valid!");
        }

        const std::pair<VkImage, VkDeviceMemory> texture_image = create_image
        (
            physical_device,
            logical_device,
            image_width,
            image_height,
            VK_FORMAT_R8G8B8A8_SRGB,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
        );

        const TextureImage image
        {
            texture_name,
            texture_image.first,
            texture_image.second
        };

        transition_image_layout(logical_device, command_pool, graphics_queue, texture_image.first, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        copy_vulkan_buffer_to_texture_image(logical_device, command_pool, graphics_queue, texture_image_buffers.get()[i - 1].buffer, texture_image.first, info);
        transition_image_layout(logical_device, command_pool, graphics_queue, texture_image.first, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        texture_images.emplace_back(image);
        log("- Texture image #" + std::to_string(i) + "/" + std::to_string(texture_image_info.size()) + " (" + force_string(texture_image.first) + ") created successfully!");
    }

    log(std::to_string(texture_image_info.size()) + " texture images created successfully!");
    return texture_images;
}

// Destroy some texture images.
void destroy_vulkan_texture_images
(
    const VkDevice &logical_device,
    std::vector<TextureImage> &texture_images
)
{
    log("Destroying " + std::to_string(texture_images.size()) + " texture images..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Texture images destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (texture_images.size() < 1)
    {
        error_log("Texture images destruction failed! No texture images were provided!");
        return;
    }

    int failed = 0;
    int i = 0;

    for (TextureImage &texture_image : texture_images)
    {
        i++;

        // Retrieve the texture image data.
        const std::string texture_name = texture_image.name;
        VkImage image = texture_image.texture_image;
        VkDeviceMemory image_memory = texture_image.image_memory;

        if (image == VK_NULL_HANDLE)
        {
            error_log("- Texture image #" + std::to_string(i) + "/" + std::to_string(texture_images.size()) + " (" + texture_name + ") destruction failed! The image provided (" + force_string(image) + ") is not valid!");
            failed++;
            continue;
        }

        if (image_memory == VK_NULL_HANDLE)
        {
            error_log("- Texture image #" + std::to_string(i) + "/" + std::to_string(texture_images.size()) + " (" + texture_name + ") destruction failed! The image memory provided (" + force_string(image_memory) + ") is not valid!");
            failed++;
            continue;
        }

        vkDestroyImage(logical_device, image, nullptr);
        image = VK_NULL_HANDLE;

        vkFreeMemory(logical_device, image_memory, nullptr);
        image_memory = VK_NULL_HANDLE;

        log("- Texture image #" + std::to_string(i) + "/" + std::to_string(texture_images.size()) + " (" + texture_name + ") destroyed successfully!");
    }

    if (failed > 0)
    {
        error_log("Warning: " + std::to_string(failed) + " texture images failed to destroy! This might lead to some memory leaks!");
    }

    log(std::to_string(texture_images.size() - failed) + "/" + std::to_string(texture_images.size()) + " texture images destroyed successfully!");
    texture_images.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_TextureImages::Vulkan_TextureImages
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const std::vector<TextureImageInfo> &texture_image_info,
    Vulkan_TextureImageBuffers &texture_image_buffers
) : logical_device(logical_device)
{
    texture_images = create_vulkan_texture_images(logical_device, physical_device, command_pool, graphics_queue, texture_image_info, texture_image_buffers);
}

// Destructor.
Vulkan_TextureImages::~Vulkan_TextureImages()
{
    destroy_vulkan_texture_images(logical_device, texture_images);
}

std::vector<TextureImage> Vulkan_TextureImages::get() const
{
    return texture_images;
}
