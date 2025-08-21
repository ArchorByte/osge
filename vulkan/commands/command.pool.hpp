#include <vulkan/vulkan.h>
#include <cstdint>

#ifndef VULKAN_COMMAND_POOL_HPP
#define VULKAN_COMMAND_POOL_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

VkCommandPool create_vulkan_command_pool
(
    const VkDevice &logical_device,
    const uint32_t &graphics_family_index
);

void destroy_vulkan_command_pool
(
    const VkDevice &logical_device,
    VkCommandPool &command_pool
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_CommandPool
{

public:
    // Constructor.
    Vulkan_CommandPool
    (
        const VkDevice &logical_device,
        const uint32_t &graphics_family_index
    );

    // Destructor.
    ~Vulkan_CommandPool();

    VkCommandPool get() const;

    // Prevent data duplication.
    Vulkan_CommandPool(const Vulkan_CommandPool&) = delete;
    Vulkan_CommandPool &operator = (const Vulkan_CommandPool&) = delete;

private:
    // We declare the members of the class to store.
    VkCommandPool command_pool = VK_NULL_HANDLE;
    VkDevice logical_device = VK_NULL_HANDLE;

};

#endif
