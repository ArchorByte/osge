#include "texture.images.loader.hpp"
#include "texture.image.buffers.hpp"

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

#ifndef VULKAN_TEXTURE_IMAGES_HANDLER_HPP
#define VULKAN_TEXTURE_IMAGES_HANDLER_HPP

///////////////////////////////////////////////////
//////////////////// Structure ////////////////////
///////////////////////////////////////////////////

struct TextureImage
{
    std::string name;
    VkImage texture_image;
    VkDeviceMemory image_memory;
    uint32_t mip_levels;
};

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

std::vector<TextureImage> create_vulkan_texture_images
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const std::vector<TextureImageInfo> &texture_image_info,
    Vulkan_TextureImageBuffers &texture_image_buffers
);

void destroy_vulkan_texture_images
(
    const VkDevice &logical_device,
    std::vector<TextureImage> &texture_images
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_TextureImages
{

public:
    // Constructor.
    Vulkan_TextureImages
    (
        const VkDevice &logical_device,
        const VkPhysicalDevice &physical_device,
        const VkCommandPool &command_pool,
        const VkQueue &graphics_queue,
        const std::vector<TextureImageInfo> &texture_image_info,
        Vulkan_TextureImageBuffers &texture_image_buffers
    );

    // Destructor.
    ~Vulkan_TextureImages();

    std::vector<TextureImage> get() const;

    // Prevent data duplication.
    Vulkan_TextureImages (const Vulkan_TextureImages&) = delete;
    Vulkan_TextureImages &operator = (const Vulkan_TextureImages&) = delete;

private:
    // We declare the members of the class to store.
    VkDevice logical_device = VK_NULL_HANDLE;
    std::vector<TextureImage> texture_images;

};

#endif
