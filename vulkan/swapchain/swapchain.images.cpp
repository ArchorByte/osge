#include "images.hpp"

#include "../../logs/terminal.hpp"
#include "../../tools/text_format.hpp"

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

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain images query failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (!swapchain || swapchain == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain images query failed! The swap chain provided (" + force_string(swapchain) + ") is not valid!");
    }

    // Count the amount of images available.
    uint32_t images_count = 0;
    VkResult first_query = vkGetSwapchainImagesKHR(logical_device, swapchain, &images_count, nullptr); // Try to count the images available.

    if (first_query != VK_SUCCESS)
    {
        fatal_error_log("Swap chain images query failed! Query 1/2 returned error code " + std::to_string(first_query) + ".");
    }

    if (images_count < 1)
    {
        fatal_error_log("Swap chain images query failed! No images were found on this swap chain!");
    }

    // Register the images into a list.
    std::vector<VkImage> swapchain_images(images_count);
    VkResult second_query = vkGetSwapchainImagesKHR(logical_device, swapchain, &images_count, swapchain_images.data()); // Try to list the images available.

    if (second_query != VK_SUCCESS)
    {
        fatal_error_log("Swap chain images query failed! Query 1/2 returned error code " + std::to_string(second_query) + ".");
    }

    if (images_count != swapchain_images.size())
    {
        fatal_error_log("Some images are missing: " + std::to_string(images_count) + " images were found but " + std::to_string(swapchain_images.size()) + " images were collected!");
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

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Image views creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (swapchain_images.size() < 1)
    {
        fatal_error_log("Image views creation failed! No swap chain images were provided!");
    }

    std::vector<VkImageView> image_views;

    // Create an image view for each swap chain image.
    for (int i = 0; i < swapchain_images.size(); i++)
    {
        // Create info for the image view
        VkImageViewCreateInfo info {};
        info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        info.image = swapchain_images[i];                             // Pass the swap chain image.
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;                        // Use 2D mode.
        info.format = swapchain_image_format;                         // Pass the image format.
        info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;            // Default red identity.
        info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;            // Default green identity.
        info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;            // Default blue identity.
        info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;            // Default alpha identity.
        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // This image view will be for the color aspect.
        info.subresourceRange.baseMipLevel = 0;                       // Set the starting mipmap level.
        info.subresourceRange.levelCount = 1;                         // Amount of mipmap levels that we will use.
        info.subresourceRange.baseArrayLayer = 0;                     // Set the starting layer.
        info.subresourceRange.layerCount = 1;                         // Amount of layers that we will use.

        VkImageView image_view = VK_NULL_HANDLE;
        VkResult view_creation = vkCreateImageView(logical_device, &info, nullptr, &image_view); // Try to create the image view.

        if (view_creation != VK_SUCCESS)
        {
            fatal_error_log("Image view #" + std::to_string(i + 1) + "/" + std::to_string(swapchain_images.size()) + " creation returned error code " + std::to_string(view_creation) + ".");
        }

        if (!image_view || image_view == VK_NULL_HANDLE)
        {
            fatal_error_log("Image view #" + std::to_string(i + 1) + "/" + std::to_string(swapchain_images.size()) + " creation output \"" + force_string(image_view) + "\" is not valid!");
        }

        image_views.emplace_back(image_view); // Register the image view.
        log("- Image view #" + std::to_string(i + 1) + "/" + std::to_string(swapchain_images.size()) + " (" + force_string(image_view) + ") created successfully!");
    }

    log(std::to_string(image_views.size()) + " image views created successfully!");
    return image_views;
}

// Destroy some image views.
void destroy_vulkan_swapchain_image_views
(
    const VkDevice &logical_device,
    std::vector<VkImageView> &images_views
)
{
    log("Destroying " + std::to_string(images_views.size()) + " swap chain image views.. ");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        error_log("Swap chain image views destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (images_views.size() < 1)
    {
        error_log("Swap chain image views destruction failed! No swap chain image views were provided!");
        return;
    }

    int failed = 0;
    int i = 0;

    // Destroy each image view in the list.
    for (VkImageView &image_view : images_views)
    {
        i++;

        if (!image_view || image_view == VK_NULL_HANDLE)
        {
            error_log("- Warning: Failed to destroy the image view #" + std::to_string(i) + "/" + std::to_string(images_views.size()) + "! The image view provided (" + force_string(image_view) + ") is not valid!");
            failed++;
            continue;
        }

        // Destroy the image view and dispose of the address.
        vkDestroyImageView(logical_device, image_view, nullptr);
        image_view = VK_NULL_HANDLE;

        log("- Image view #" + std::to_string(i) + "/" + std::to_string(images_views.size()) + " destroyed successfully!");
    }

    if (failed > 0) error_log("Warning: " + std::to_string(failed) + " swap chain image views failed to destroy! This might lead to some memory leaks or memory overload.");
    log(std::to_string(images_views.size() - failed) + "/" + std::to_string(images_views.size()) + " swap chain image views destroyed successfully!");

    // Free the list.
    images_views.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_ImagesViews::Vulkan_ImagesViews
(
    const VkDevice &logical_device,
    const std::vector<VkImage> &swapchain_images,
    const VkFormat &swapchain_image_format
) : logical_device(logical_device)
{
    images_views = create_vulkan_swapchain_image_views(logical_device, swapchain_images, swapchain_image_format);
}

// Destructor.
Vulkan_ImagesViews::~Vulkan_ImagesViews()
{
    destroy_vulkan_swapchain_image_views(logical_device, images_views);
}

std::vector<VkImageView> Vulkan_ImagesViews::get() const
{
    return images_views;
}
