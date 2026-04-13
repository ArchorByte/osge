#include "vulkan.swapchain.hpp"

#include "../images/vulkan.images.hpp"
#include "../../../utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"

#include <cstdint>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Get swap chain images.

    Tasks:
        1) Verify function parameters.
        2) Try to get the amount of swap images.
        3) Try to retrieve the swap chain images.

    Parameters:
        - logical_device / VkDevice       / Logical device of the Vulkan instance.
        - swapchain      / VkSwapchainKHR / Swap chain to retrieve images from.

    Returns:
        A vector list containing all swap chain images retrieved.
*/
std::vector<VkImage> Swapchain::get_swapchain_images
(
    const VkDevice       &logical_device,
    const VkSwapchainKHR &swapchain
)
{
    Utils::Logs::log("Retrieving swap chain images.. ", false);

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (swapchain == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Swap chain invalid.");

    uint32_t images_count = 0;
    const VkResult first_query = vkGetSwapchainImagesKHR(logical_device, swapchain, &images_count, nullptr);

    if (first_query != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Query #1 returned error code -> " + std::to_string(first_query) + ".");

    if (images_count < 1)
        Utils::Logs::crash_log("Failed! No images were found.");

    std::vector<VkImage> swapchain_images(images_count);
    const VkResult second_query = vkGetSwapchainImagesKHR(logical_device, swapchain, &images_count, swapchain_images.data());

    if (second_query != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Query #2 returned error code -> " + std::to_string(second_query) + ".");

    Utils::Logs::log("Done! " + std::to_string(images_count) + " images retrieved.", true);
    return swapchain_images;
}



/*
    Create image views for each swap chain image.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Create an image view for each swap chain image.

    Parameters:
        - image_format     / VkFormat        / Format of the swap chain images.
        - logical_device   / VkDevice        / Logical device of the Vulkan instance.
        - swapchain_images / vector<VkImage> / All swap chain images used to create image views.

    Returns:
        A vector list containing all created image views.
*/
std::vector<VkImageView> Swapchain::create_swapchain_image_views
(
    const VkFormat             &image_format,
    const VkDevice             &logical_device,
    const std::vector<VkImage> &swapchain_images
)
{
    Utils::Logs::log("Creating " + std::to_string(swapchain_images.size()) + " swap chain image views.. ", false);

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (swapchain_images.size() < 1)
        Utils::Logs::crash_log("Failed! No swap chain images provided.");

    std::vector<VkImageView> image_views;
    image_views.reserve(swapchain_images.size());

    for (int i = 0; i < swapchain_images.size(); i++)
    {
        const VkImageView image_view = Images::create_image_view(VK_IMAGE_ASPECT_COLOR_BIT, image_format, swapchain_images[i], logical_device, 1);
        image_views.emplace_back(image_view);
    }

    Utils::Logs::log("Done!", true);
    return image_views;
}



/*
    Destroy some swap chain image views.

    Tasks:
        1) Verify function parameters.
        2) Destroy all valid image views.
        3) Set all objects to null.
        4) Empty vector list.

    Parameters:
        - image_views    / vector<VkImageView> / Swap chain image views to destroy.
        - logical_device / VkDevice            / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Swapchain::destroy_swapchain_image_views
(
    std::vector<VkImageView> &image_views,
    const VkDevice           &logical_device
)
{
    Utils::Logs::log("Destroying " + std::to_string(image_views.size()) + " swap chain image views.. ", false);

    if (image_views.size() < 1)
    {
        Utils::Logs::log("Done!", true);
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    for (VkImageView &image_view : image_views)
    {
        if (image_view == VK_NULL_HANDLE)
            continue;

        vkDestroyImageView(logical_device, image_view, nullptr);
        image_view = VK_NULL_HANDLE;
    }

    Utils::Logs::log("Done!", true);
    image_views.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Swapchain::swapchain_image_views_handler::swapchain_image_views_handler
(
    const VkFormat             &image_format,
    const VkDevice             &logical_device,
    const std::vector<VkImage> &swapchain_images
)
    : logical_device(logical_device)
{
    swapchain_image_views = create_swapchain_image_views(image_format, logical_device, swapchain_images);
}

Swapchain::swapchain_image_views_handler::~swapchain_image_views_handler()
{
    destroy_swapchain_image_views(swapchain_image_views, logical_device);
}

std::vector<VkImageView> Swapchain::swapchain_image_views_handler::get() const
{
    return swapchain_image_views;
}
