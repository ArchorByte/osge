#include "texture.image.views.hpp"

#include "texture.images.handler.hpp"
#include "../images/image.views.handler.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create views for the texture images.
std::vector<VkImageView> create_vulkan_texture_image_views
(
    const VkDevice &logical_device,
    const std::vector<TextureImage> &texture_images
)
{
    log("Creating " + std::to_string(texture_images.size()) + " texture image views..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Texture image views creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (texture_images.size() < 1)
    {
        fatal_error_log("Texture image views creation failed! No texture images were provided!");
    }

    std::vector<VkImageView> image_views;
    image_views.reserve(texture_images.size());

    for (int i = 0; i < texture_images.size(); i++)
    {
        const VkImageView image_view = create_image_view(logical_device, texture_images[i].texture_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
        image_views.emplace_back(image_view);
        log("- Texture image view #" + std::to_string(i + 1) + "/" + std::to_string(texture_images.size()) + " (" + force_string(image_view) + ") created successfully!");
    }

    log(std::to_string(texture_images.size()) + " texture image views created successfully!");
    return image_views;
}

// Destroy some texture image views.
void destroy_vulkan_texture_image_views
(
    const VkDevice &logical_device,
    std::vector<VkImageView> &texture_image_views
)
{
    log("Destroying " + std::to_string(texture_image_views.size()) + " texture image views..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Texture image views destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (texture_image_views.size() < 1)
    {
        error_log("Texture image views destruction failed! No texture image views were provided!");
        return;
    }

    int failed = 0;
    int i = 0;

    for (VkImageView &texture_image_view : texture_image_views)
    {
        i++;

        if (texture_image_view == VK_NULL_HANDLE)
        {
            error_log("- Failed to destroy the texture image view #" + std::to_string(i) + "/" + std::to_string(texture_image_views.size()) + "! The image view provided (" + force_string(texture_image_view) + ") is not valid!");
            failed++;
            continue;
        }

        vkDestroyImageView(logical_device, texture_image_view, nullptr);
        texture_image_view = VK_NULL_HANDLE;

        log("- Texture image view #" + std::to_string(i) + "/" + std::to_string(texture_image_views.size()) + " destroyed successfully!");
    }

    if (failed > 0)
    {
        error_log("Warning: " + std::to_string(failed) + " texture image views failed to destroy! This might lead to some memory leaks or memory overload.");
    }

    log(std::to_string(texture_image_views.size() - failed) + "/" + std::to_string(texture_image_views.size()) + " texture image views destroyed successfully!");
    texture_image_views.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_TextureImageViews::Vulkan_TextureImageViews
(
    const VkDevice &logical_device,
    const std::vector<TextureImage> &texture_images
) : logical_device(logical_device)
{
    texture_image_views = create_vulkan_texture_image_views(logical_device, texture_images);
}

// Destructor.
Vulkan_TextureImageViews::~Vulkan_TextureImageViews()
{
    destroy_vulkan_texture_image_views(logical_device, texture_image_views);
}

std::vector<VkImageView> Vulkan_TextureImageViews::get() const
{
    return texture_image_views;
}
