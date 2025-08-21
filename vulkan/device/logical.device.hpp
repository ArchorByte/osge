#include <vulkan/vulkan.h>
#include <vector>

#ifndef VULKAN_LOGICAL_DEVICE_HPP
#define VULKAN_LOGICAL_DEVICE_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

VkDevice create_logical_device
(
    const VkPhysicalDevice &physical_device,
    const std::vector<VkDeviceQueueCreateInfo> &queues_create_info,
    const std::vector<const char *> &required_extensions
);

void destroy_logical_device
(
    VkDevice &logical_device
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_LogicalDevice
{

public:
    // Constructor.
    Vulkan_LogicalDevice
    (
        const VkPhysicalDevice &physical_device,
        const std::vector<VkDeviceQueueCreateInfo> &queues_create_info,
        const std::vector<const char *> &required_extensions
    );

    // Destructor.
    ~Vulkan_LogicalDevice();

    VkDevice get() const;

    // Prevent data duplication.
    Vulkan_LogicalDevice(const Vulkan_LogicalDevice&) = delete;
    Vulkan_LogicalDevice &operator = (const Vulkan_LogicalDevice&) = delete;

private:
    // We declare the members of the class to store.
    VkDevice logical_device = VK_NULL_HANDLE;

};

#endif
