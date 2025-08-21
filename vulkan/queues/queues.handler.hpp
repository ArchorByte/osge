#include <vector>
#include <vulkan/vulkan.h>

#ifndef VULKAN_QUEUES_HANDLER_HPP
#define VULKAN_QUEUES_HANDLER_HPP

std::vector<VkQueueFamilyProperties> get_queue_families
(
    const VkPhysicalDevice &physical_device
);

std::vector<VkDeviceQueueCreateInfo> make_queues_create_info
(
    const VkPhysicalDevice &physical_device,
    const std::vector<uint32_t> &required_queues_indexes,
    const float &queue_priority
);

#endif
