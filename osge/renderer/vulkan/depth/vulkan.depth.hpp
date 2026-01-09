#ifndef VULKAN_DEPTH_HPP
#define VULKAN_DEPTH_HPP

#include <vector>
#include <vulkan/vulkan.h>

///////////////////////////////////////////////////
//////////////////// Structure ////////////////////
///////////////////////////////////////////////////

struct DepthResources
{
    VkImage depth_image;
    VkDeviceMemory image_memory;
    VkImageView image_view;
};

///////////////////////////////////////////////////
//////////////////// Namespace ////////////////////
///////////////////////////////////////////////////

namespace Vulkan::Depth
{
    ///////////////////////////////////////////////////
    //////////////////// Functions ////////////////////
    ///////////////////////////////////////////////////

    /////////////////////////////////
    ///// depth.attachments.cpp /////
    /////////////////////////////////

    VkAttachmentDescription create_depth_attachment
    (
        const VkPhysicalDevice &physical_device,
        const VkSampleCountFlagBits &samples_count
    );

    /////////////////////////////
    ///// depth.formats.cpp /////
    /////////////////////////////

    VkFormat find_depth_format
    (
        const VkPhysicalDevice &physical_device
    );

    /////////////////////////////
    ///// depth.resources.cpp /////
    /////////////////////////////

    DepthResources create_depth_resources
    (
        const VkCommandPool &command_pool,
        const VkExtent2D &extent,
        const VkQueue &graphics_queue,
        const VkDevice &logical_device,
        const VkPhysicalDevice &physical_device,
        const VkSampleCountFlagBits &samples_count
    );

    void destroy_depth_resources
    (
        DepthResources &depth_resources,
        const VkDevice &logical_device
    );

    ///////////////////////////////////////////////
    //////////////////// Class ////////////////////
    ///////////////////////////////////////////////

    class depth_resources_handler
    {
        public:
            depth_resources_handler
            (
                const VkCommandPool &command_pool,
                const VkExtent2D &extent,
                const VkQueue &graphics_queue,
                const VkDevice &logical_device,
                const VkPhysicalDevice &physical_device,
                const VkSampleCountFlagBits &samples_count
            );

            ~depth_resources_handler();
            DepthResources get() const;

            // Prevent data duplication.
            depth_resources_handler(const depth_resources_handler&) = delete;
            depth_resources_handler &operator = (const depth_resources_handler&) = delete;
        private:
            DepthResources depth_resources;
            VkDevice logical_device = VK_NULL_HANDLE;
    };
}

#endif
