#include <vulkan/vulkan.h>
#include <vector>

#ifndef VULKAN_DESCRIPTOR_SET_LAYOUT_HPP
#define VULKAN_DESCRIPTOR_SET_LAYOUT_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

VkDescriptorSetLayout create_vulkan_descriptor_set_layout
(
    const VkDevice &logical_device,
    const std::vector<VkImageView> &texture_image_views
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
        const VkDevice &logical_device,
        const std::vector<VkImageView> &texture_image_views
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
