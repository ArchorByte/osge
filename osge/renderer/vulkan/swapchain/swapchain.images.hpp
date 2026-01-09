#include <vulkan/vulkan.h>
#include <vector>

#ifndef VULKAN_SWAPCHAIN_IMAGES_HPP
#define VULKAN_SWAPCHAIN_IMAGES_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

std::vector<VkImage> get_vulkan_swapchain_images
(
    const VkDevice &logical_device,
    const VkSwapchainKHR &swapchain
);

std::vector<VkImageView> create_vulkan_swapchain_image_views
(
    const VkDevice &logical_device,
    const std::vector<VkImage> &swapchain_images,
    const VkFormat &swapchain_image_format
);

void destroy_vulkan_swapchain_image_views
(
    const VkDevice &logical_device,
    std::vector<VkImageView> &images_views
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_SwapchainImageViews
{

public:
    // Constructor.
    Vulkan_SwapchainImageViews
    (
        const VkDevice &logical_device,
        const std::vector<VkImage> &swapchain_images,
        const VkFormat &swapchain_image_format
    );

    // Destructor.
    ~Vulkan_SwapchainImageViews();

    std::vector<VkImageView> get() const;

    // Prevent data duplication.
    Vulkan_SwapchainImageViews(const Vulkan_SwapchainImageViews&) = delete;
    Vulkan_SwapchainImageViews &operator = (const Vulkan_SwapchainImageViews&) = delete;

private:
    // We declare the members of the class to store.
    std::vector<VkImageView> swapchain_image_views;
    VkDevice logical_device = VK_NULL_HANDLE;

};

#endif
