#include <vulkan/vulkan.h>
#include <vector>

#ifndef VULKAN_GRAPHICS_PIPELINE_HPP
#define VULKAN_GRAPHICS_PIPELINE_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

VkPipeline create_vulkan_graphics_pipeline
(
    const VkDevice &logical_device,
    const std::vector<VkPipelineShaderStageCreateInfo> &pipeline_shaders_stages,
    const VkPipelineVertexInputStateCreateInfo &vertex_input_state,
    const VkPipelineInputAssemblyStateCreateInfo &assembly_input_state,
    const VkPipelineViewportStateCreateInfo &viewport_state,
    const VkPipelineRasterizationStateCreateInfo &rasterization_state,
    const VkPipelineMultisampleStateCreateInfo &multisampling_state,
    const VkPipelineLayout &pipeline_layout,
    const VkRenderPass &render_pass,
    const VkPipelineDynamicStateCreateInfo &dynamic_state
);

void destroy_vulkan_graphics_pipeline
(
    const VkDevice &logical_device,
    VkPipeline &graphics_pipeline
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_GraphicsPipeline
{

public:
    // Constructor.
    Vulkan_GraphicsPipeline
    (
        const VkDevice &logical_device,
        const std::vector<VkPipelineShaderStageCreateInfo> &pipeline_shaders_stages,
        const VkPipelineVertexInputStateCreateInfo &vertex_input_state,
        const VkPipelineInputAssemblyStateCreateInfo &assembly_input_state,
        const VkPipelineViewportStateCreateInfo &viewport_state,
        const VkPipelineRasterizationStateCreateInfo &rasterization_state,
        const VkPipelineMultisampleStateCreateInfo &multisampling_state,
        const VkPipelineLayout &pipeline_layout,
        const VkRenderPass &render_pass,
        const VkPipelineDynamicStateCreateInfo &dynamic_state
    );

    // Destructor.
    ~Vulkan_GraphicsPipeline();

    VkPipeline get() const;

    // Prevent data duplication.
    Vulkan_GraphicsPipeline(const Vulkan_GraphicsPipeline&) = delete;
    Vulkan_GraphicsPipeline &operator = (const Vulkan_GraphicsPipeline&) = delete;

private:
    // We declare the members of the class to store.
    VkPipeline graphics_pipeline = VK_NULL_HANDLE;
    VkDevice logical_device = VK_NULL_HANDLE;

};

#endif
