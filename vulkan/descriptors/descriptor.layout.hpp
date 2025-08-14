#include <vulkan/vulkan.h>

#ifndef VULKAN_UNIFORM_DESCRIPTOR_LAYOUT_HPP
#define VULKAN_UNIFORM_DESCRIPTOR_LAYOUT_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

VkDescriptorSetLayout create_vulkan_descriptor_set_layout
(
    const VkDevice &logical_device
);

void destroy_vulkan_descriptor_set_layout
(
    const VkDevice &logical_device,
    VkDescriptorSetLayout &descriptor_set_layout
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_DescriptorSetLayout
{

public:
    // Constructor.
    Vulkan_DescriptorSetLayout
    (
        const VkDevice &logical_device
    );

    // Destructor.
    ~Vulkan_DescriptorSetLayout();

    VkDescriptorSetLayout get() const;

    // Prevent data duplication.
    Vulkan_DescriptorSetLayout(const Vulkan_DescriptorSetLayout&) = delete;
    Vulkan_DescriptorSetLayout &operator = (const Vulkan_DescriptorSetLayout&) = delete;

private:
    // We declare the members of the class to store.
    VkDevice logical_device = VK_NULL_HANDLE;
    VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;

};

#endif
