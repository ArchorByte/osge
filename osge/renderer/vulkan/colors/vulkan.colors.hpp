#ifndef VULKAN_COLORS_HPP
#define VULKAN_COLORS_HPP

#include <vulkan/vulkan.h>

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
//////////////////// Namespace ////////////////////
///////////////////////////////////////////////////

namespace Vulkan::Colors
{
    ////////////////////////////////
    ///// color.attachment.cpp /////
    ////////////////////////////////

    VkAttachmentDescription create_color_attachment
    (
        const VkSampleCountFlagBits &samples_count,
        const VkFormat &surface_format
    );

    ///////////////////////////////
    ///// color.resources.cpp /////
    ///////////////////////////////

    ColorResources create_color_resources
    (
        const VkDevice &logical_device,
        const VkPhysicalDevice &physical_device,
        const VkSampleCountFlagBits &samples_count,
        const VkExtent2D &swapchain_extent,
        const VkFormat &swapchain_image_format
    );

    void destroy_color_resources
    (
        ColorResources &color_resources,
        const VkDevice &logical_device
    );

    ///////////////////////////////////////////////
    //////////////////// Class ////////////////////
    ///////////////////////////////////////////////

    class color_resources_handler
    {
        public:
            color_resources_handler
            (
                const VkDevice &logical_device,
                const VkPhysicalDevice &physical_device,
                const VkSampleCountFlagBits &samples_count,
                const VkExtent2D &swapchain_extent,
                const VkFormat &swapchain_image_format
            );

            ~color_resources_handler();
            ColorResources get() const;

            // Prevent data duplication.
            color_resources_handler(const color_resources_handler&) = delete;
            color_resources_handler &operator = (const color_resources_handler&) = delete;
        private:
            ColorResources color_resources;
            VkDevice logical_device = VK_NULL_HANDLE;
    };
}

#endif
