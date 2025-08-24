#include <vulkan/vulkan.h>
#include <cstdint>

#ifndef VULKAN_DESCRIPTOR_POOL_HPP
#define VULKAN_DESCRIPTOR_POOL_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

VkDescriptorPool create_vulkan_descriptor_pool
(
    const VkDevice &logical_device,
    const uint32_t &images_count,
    const uint32_t &texture_images_count
);

void destroy_vulkan_descriptor_pool
(
    const VkDevice &logical_device,
    VkDescriptorPool &descriptor_pool
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_DescriptorPool
{

public:
    // Constructor.
    Vulkan_DescriptorPool
    (
        const VkDevice &logical_device,
        const uint32_t &images_count,
        const uint32_t &texture_images_count
    );

    // Destructor.
    ~Vulkan_DescriptorPool();

    VkDescriptorPool get() const;

    // Prevent data duplication.
    Vulkan_DescriptorPool(const Vulkan_DescriptorPool&) = delete;
    Vulkan_DescriptorPool &operator = (const Vulkan_DescriptorPool&) = delete;

private:
    // We declare the members of the class to store.
    VkDevice logical_device = VK_NULL_HANDLE;
    VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;

};

#endif
