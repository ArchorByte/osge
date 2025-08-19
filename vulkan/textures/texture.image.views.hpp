#include "texture.images.handler.hpp"

#include <vulkan/vulkan.h>
#include <vector>

#ifndef TEXTURE_IMAGE_VIEWS_HPP
#define TEXTURE_IMAGE_VIEWS_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

std::vector<VkImageView> create_vulkan_texture_image_views
(
    const VkDevice &logical_device,
    const std::vector<TextureImage> &texture_images
);

void destroy_vulkan_texture_image_views
(
    const VkDevice &logical_device,
    std::vector<VkImageView> &texture_image_views
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_TextureImageViews
{

public:
    // Constructor.
    Vulkan_TextureImageViews
    (
        const VkDevice &logical_device,
        const std::vector<TextureImage> &texture_images
    );

    // Destructor.
    ~Vulkan_TextureImageViews();

    std::vector<VkImageView> get() const;

    // Prevent data duplication.
    Vulkan_TextureImageViews(const Vulkan_TextureImageViews&) = delete;
    Vulkan_TextureImageViews &operator = (const Vulkan_TextureImageViews&) = delete;

private:
    // We declare the members of the class to store.
    std::vector<VkImageView> texture_image_views;
    VkDevice logical_device = VK_NULL_HANDLE;

};

#endif
