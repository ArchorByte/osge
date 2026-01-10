#ifndef VULKAN_IMAGES_HPP
#define VULKAN_IMAGES_HPP

#include <utility>
#include <libraries/vulkan/vulkan.h>

namespace Vulkan::Images
{
    /////////////////////////////////
    ///// image.transitions.cpp /////
    /////////////////////////////////

    void transition_image_layout
    (
        const VkCommandPool &command_pool,
        const VkFormat &format,
        const VkQueue &graphics_queue,
        const VkImage &image,
        const VkDevice &logical_device,
        const uint32_t &mip_levels,
        const VkImageLayout &new_layout,
        const VkImageLayout &old_layout
    );

    ///////////////////////////////////
    ///// image.views.handler.cpp /////
    ///////////////////////////////////

    VkImageView create_image_view
    (
        const VkImageAspectFlags &aspect_flags,
        const VkFormat &format,
        const VkImage &image,
        const VkDevice &logical_device,
        const uint32_t &mip_levels
    );

    //////////////////////////////
    ///// images.handler.cpp /////
    //////////////////////////////

    std::pair<VkImage, VkDeviceMemory> create_image
    (
        const VkFormat &format,
        const int &height,
        const VkImageTiling &image_tiling,
        const VkDevice &logical_device,
        const uint32_t &mip_levels,
        const VkPhysicalDevice &physical_device,
        const VkSampleCountFlagBits &samples_count,
        const VkImageUsageFlags &usage_flags,
        const int &width
    );
}

#endif
