#include <vulkan/vulkan.h>

#ifndef VULKAN_DEPTH_RESOURCES_HPP
#define VULKAN_DEPTH_RESOURCES_HPP

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
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

DepthResources create_depth_resources
(
    const VkPhysicalDevice &physical_device,
    const VkDevice &logical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkExtent2D &extent
);

void destroy_depth_resources
(
    const VkDevice &logical_device,
    DepthResources &depth_resources
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_DepthResources
{

public:
    // Constructor.
    Vulkan_DepthResources
    (
        const VkPhysicalDevice &physical_device,
        const VkDevice &logical_device,
        const VkCommandPool &command_pool,
        const VkQueue &graphics_queue,
        const VkExtent2D &extent
    );

    // Destructor.
    ~Vulkan_DepthResources();

    DepthResources get() const;

    // Prevent data duplication.
    Vulkan_DepthResources(const Vulkan_DepthResources&) = delete;
    Vulkan_DepthResources &operator = (const Vulkan_DepthResources&) = delete;

private:
    DepthResources depth_resources;
    VkDevice logical_device = VK_NULL_HANDLE;

};

#endif
