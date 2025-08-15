#include "../uniform/uniform.buffers.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

#ifndef VULKAN_UNIFORM_DESCRIPTOR_SETS_HPP
#define VULKAN_UNIFORM_DESCRIPTOR_SETS_HPP

std::vector<VkDescriptorSet> create_vulkan_descriptor_sets
(
    const VkDevice &logical_device,
    const uint32_t &images_count,
    const VkDescriptorSetLayout &descriptor_set_layout,
    const VkDescriptorPool &descriptor_pool,
    const std::vector<UniformBufferInfo> &uniform_buffers
);

#endif
