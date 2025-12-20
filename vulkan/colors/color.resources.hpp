#include <vulkan/vulkan.h>

#ifndef VULKAN_COLOR_RESOURCES_HPP
#define VULKAN_COLOR_RESOURCES_HPP

///////////////////////////////////////////////////
//////////////////// Structure ////////////////////
///////////////////////////////////////////////////

struct ColorResources
{
    VkImage color_image;
    VkDeviceMemory color_image_memory;
    VkImageView color_image_view;
};

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

ColorResources create_color_resources
(
    const VkPhysicalDevice &physical_device,
    const VkDevice &logical_device,
    const VkExtent2D &swapchain_extent,
    const VkFormat &swapchain_image_format,
    const VkSampleCountFlagBits &samples_count
);

void destroy_color_resources
(
    const VkDevice &logical_device,
    ColorResources &color_resources
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_ColorResources
{

public:
    // Constructor.
    Vulkan_ColorResources
    (
        const VkPhysicalDevice &physical_device,
        const VkDevice &logical_device,
        const VkExtent2D &swapchain_extent,
        const VkFormat &swapchain_image_format,
        const VkSampleCountFlagBits &samples_count
    );

    // Destructor.
    ~Vulkan_ColorResources();

    ColorResources get() const;

    // Prevent data duplication.
    Vulkan_ColorResources(const Vulkan_ColorResources&) = delete;
    Vulkan_ColorResources &operator = (const Vulkan_ColorResources&) = delete;

private:
    ColorResources color_resources;
    VkDevice logical_device = VK_NULL_HANDLE;

};

#endif
