#include "loader.hpp"

#include <vulkan/vulkan.h>
#include <vector>

#ifndef VULKAN_TEXTURES_BUFFER_HPP
#define VULKAN_TEXTURES_BUFFER_HPP

///////////////////////////////////////////////////
//////////////////// Structure ////////////////////
///////////////////////////////////////////////////

struct Buffer
{
    VkBuffer buffer;
    VkDeviceMemory buffer_memory;
};

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

std::vector<Buffer> create_vulkan_texture_image_buffers
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const std::vector<TextureImageInfo> &texture_image_info
);

void destroy_vulkan_texture_image_buffers
(
    const VkDevice &logical_device,
    std::vector<Buffer> &texture_image_buffers
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_TextureImageBuffers
{

public:
    // Constructor.
    Vulkan_TextureImageBuffers
    (
        const VkDevice &logical_device,
        const VkPhysicalDevice &physical_device,
        const std::vector<TextureImageInfo> &texture_image_info
    );

    // Destructor.
    ~Vulkan_TextureImageBuffers();

    std::vector<Buffer> get() const;

    // Prevent data duplication.
    Vulkan_TextureImageBuffers (const Vulkan_TextureImageBuffers&) = delete;
    Vulkan_TextureImageBuffers &operator = (const Vulkan_TextureImageBuffers&) = delete;

private:
    // We declare the members of the class to store.
    VkDevice logical_device = VK_NULL_HANDLE;
    std::vector<Buffer> texture_image_buffers;

};

#endif
