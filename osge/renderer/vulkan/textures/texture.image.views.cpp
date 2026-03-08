#include "vulkan.textures.hpp"

#include "libraries/vulkan/vulkan.h"
#include "../images/vulkan.images.hpp"
#include "../../../utils/utils.hpp"

#include <string>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create views for texture images.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Reserve memory for the vector list.
        3) Create a view for each texture image.

    Parameters:
        - logical_device / VkDevice                 / Logical device of the Vulkan instance.
        - texture_images / vector<TextureImageData> / Texture images that are used for the views creation.

    Returns:
        A vector list containing all create texture image views.
*/
std::vector<VkImageView> Textures::create_texture_image_views
(
    const VkDevice                      &logical_device,
    const std::vector<TextureImageData> &texture_images
)
{
    Utils::Logs::log("Creating " + std::to_string(texture_images.size()) + " texture image views.. ", false);

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (texture_images.size() < 1)
        Utils::Logs::crash_log("Failed! No texture images provided.");

    std::vector<VkImageView> image_views;
    image_views.reserve(texture_images.size());

    for (int i = 0; i < texture_images.size(); i++)
    {
        const VkImageView image_view = Images::create_image_view(VK_IMAGE_ASPECT_COLOR_BIT, VK_FORMAT_R8G8B8A8_SRGB, texture_images[i].texture_image, logical_device, texture_images[i].mip_levels);
        image_views.emplace_back(image_view);
    }

    Utils::Logs::log("Done! " + std::to_string(texture_images.size()) + " texture image views created.", true);
    return image_views;
}



/*
    Destroy texture image views.

    Tasks:
        1) Verify function parameters.
        2) Destroy each texture image view.

    Parameters:
        - logical_device      / VkDevice            / Logical device of the Vulkan instance.
        - texture_image_views / vector<VkImageView> / Texture image views to destroy.

    Returns:
        No object returned.
*/
void Textures::destroy_texture_image_views
(
    const VkDevice           &logical_device,
    std::vector<VkImageView> &texture_image_views
)
{
    Utils::Logs::log("Destroying " + std::to_string(texture_image_views.size()) + " texture image views.. ", false);

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    if (texture_image_views.size() < 1)
    {
        Utils::Logs::log("Done!", true);
        return;
    }

    int failed = 0;
    int i = 0;

    for (VkImageView &texture_image_view : texture_image_views)
    {
        i++;

        if (texture_image_view == VK_NULL_HANDLE)
        {
            failed++;
            continue;
        }

        vkDestroyImageView(logical_device, texture_image_view, nullptr);
        texture_image_view = VK_NULL_HANDLE;
    }

    if (failed > 0)
        Utils::Logs::log("Done! Warning: " + std::to_string(failed) + " destructions failed.", true);
    else
        Utils::Logs::log("Done!", true);

    texture_image_views.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Textures::texture_image_views_handler::texture_image_views_handler
(
    const VkDevice                      &logical_device,
    const std::vector<TextureImageData> &texture_images
)
    : logical_device(logical_device)
{
    texture_image_views = create_texture_image_views(logical_device, texture_images);
}

Textures::texture_image_views_handler::~texture_image_views_handler()
{
    destroy_texture_image_views(logical_device, texture_image_views);
}

std::vector<VkImageView> Textures::texture_image_views_handler::get() const
{
    return texture_image_views;
}
