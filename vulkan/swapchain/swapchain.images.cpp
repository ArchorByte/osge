#include "swapchain.images.hpp"

#include "../images/image.views.handler.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Return the list of images of a swap chain.
std::vector<VkImage> get_vulkan_swapchain_images
(
    const VkDevice &logical_device,
    const VkSwapchainKHR &swapchain
)
{
    log("Fetching the images of a swap chain..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain images query failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (swapchain == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain images query failed! The swap chain provided (" + force_string(swapchain) + ") is not valid!");
    }

    uint32_t images_count = 0;
    const VkResult first_query = vkGetSwapchainImagesKHR(logical_device, swapchain, &images_count, nullptr);

    if (first_query != VK_SUCCESS)
    {
        fatal_error_log("Swap chain images query failed! Query 1/2 returned error code " + std::to_string(first_query) + ".");
    }

    if (images_count < 1)
    {
        fatal_error_log("Swap chain images query failed! No images were found on this swap chain!");
    }

    std::vector<VkImage> swapchain_images(images_count);
    const VkResult second_query = vkGetSwapchainImagesKHR(logical_device, swapchain, &images_count, swapchain_images.data());

    if (second_query != VK_SUCCESS)
    {
        fatal_error_log("Swap chain images query failed! Query 1/2 returned error code " + std::to_string(second_query) + ".");
    }

    if (images_count != swapchain_images.size())
    {
        fatal_error_log("Swap chain images query failed! Some images are missing: " + std::to_string(images_count) + " images were found but " + std::to_string(swapchain_images.size()) + " images were retrieved!");
    }

    log(std::to_string(images_count) + " swap chain images retrieved successfully!");
    return swapchain_images;
}

// Create an image view for each swap chain image.
std::vector<VkImageView> create_vulkan_swapchain_image_views
(
    const VkDevice &logical_device,
    const std::vector<VkImage> &swapchain_images,
    const VkFormat &swapchain_image_format
)
{
    log("Creating " + std::to_string(swapchain_images.size()) + " swap chain image views..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain image views creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (swapchain_images.size() < 1)
    {
        fatal_error_log("Swap chain image views creation failed! No swap chain images were provided!");
    }

    std::vector<VkImageView> image_views;
    image_views.reserve(swapchain_images.size());

    for (int i = 0; i < swapchain_images.size(); i++)
    {
        const VkImageView image_view = create_image_view(logical_device, swapchain_images[i], swapchain_image_format, VK_IMAGE_ASPECT_COLOR_BIT);
        image_views.emplace_back(image_view);
        log("- Swap chain image view #" + std::to_string(i + 1) + "/" + std::to_string(swapchain_images.size()) + " (" + force_string(image_view) + ") created successfully!");
    }

    log(std::to_string(image_views.size()) + " swap chain image views created successfully!");
    return image_views;
}

// Destroy some swap chain image views.
void destroy_vulkan_swapchain_image_views
(
    const VkDevice &logical_device,
    std::vector<VkImageView> &image_views
)
{
    log("Destroying " + std::to_string(image_views.size()) + " swap chain image views.. ");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Swap chain image views destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (image_views.size() < 1)
    {
        error_log("Swap chain image views destruction failed! No swap chain image views were provided!");
        return;
    }

    int failed = 0;
    int i = 0;

    for (VkImageView &image_view : image_views)
    {
        i++;

        if (image_view == VK_NULL_HANDLE)
        {
            error_log("- Failed to destroy the swap chain image view #" + std::to_string(i) + "/" + std::to_string(image_views.size()) + "! The image view provided (" + force_string(image_view) + ") is not valid!");
            failed++;
            continue;
        }

        vkDestroyImageView(logical_device, image_view, nullptr);
        image_view = VK_NULL_HANDLE;

        log("- Swap chain image view #" + std::to_string(i) + "/" + std::to_string(image_views.size()) + " destroyed successfully!");
    }

    if (failed > 0)
    {
        error_log("Warning: " + std::to_string(failed) + " swap chain image views failed to destroy! This might lead to some memory leaks or memory overload.");
    }

    log(std::to_string(image_views.size() - failed) + "/" + std::to_string(image_views.size()) + " swap chain image views destroyed successfully!");
    image_views.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_SwapchainImageViews::Vulkan_SwapchainImageViews
(
    const VkDevice &logical_device,
    const std::vector<VkImage> &swapchain_images,
    const VkFormat &swapchain_image_format
) : logical_device(logical_device)
{
    swapchain_image_views = create_vulkan_swapchain_image_views(logical_device, swapchain_images, swapchain_image_format);
}

// Destructor.
Vulkan_SwapchainImageViews::~Vulkan_SwapchainImageViews()
{
    destroy_vulkan_swapchain_image_views(logical_device, swapchain_image_views);
}

std::vector<VkImageView> Vulkan_SwapchainImageViews::get() const
{
    return swapchain_image_views;
}
