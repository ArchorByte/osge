#include <vulkan/vulkan.h>

#ifndef VULKAN_PIPELINE_LAYOUT_HPP
#define VULKAN_PIPELINE_LAYOUT_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

VkPipelineLayout create_vulkan_pipeline_layout
(
    const VkDevice &logical_device,
    const VkDescriptorSetLayout &descriptor_set_layout
);

void destroy_vulkan_pipeline_layout
(
    const VkDevice &logical_device,
    VkPipelineLayout &pipeline_layout
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_PipelineLayout
{

public:
    // Constructor.
    Vulkan_PipelineLayout
    (
        const VkDevice &logical_device,
    const VkDescriptorSetLayout &descriptor_set_layout
    );

    // Destructor.
    ~Vulkan_PipelineLayout();

    VkPipelineLayout get() const;

    // Prevent data duplication.
    Vulkan_PipelineLayout(const Vulkan_PipelineLayout&) = delete;
    Vulkan_PipelineLayout &operator = (const Vulkan_PipelineLayout&) = delete;

private:
    // We declare the members of the class to store.
    VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
    VkDevice logical_device = VK_NULL_HANDLE;

};

#endif
