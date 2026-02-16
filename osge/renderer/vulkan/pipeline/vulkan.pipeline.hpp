#ifndef VULKAN_PIPELINE_HPP
#define VULKAN_PIPELINE_HPP

#include "libraries/vulkan/vulkan.h"
#include <vector>

namespace Pipeline
{
    ///////////////////////////////////////////////////
    //////////////////// Functions ////////////////////
    ///////////////////////////////////////////////////

    /////////////////////////////////
    ///// graphics.pipeline.cpp /////
    /////////////////////////////////

    VkPipeline create_graphics_pipeline
    (
        const VkPipelineInputAssemblyStateCreateInfo &assembly_input_state,
        const VkPipelineDynamicStateCreateInfo &dynamic_state,
        const VkDevice &logical_device,
        const VkPipelineMultisampleStateCreateInfo &multisampling_state,
        const VkPipelineLayout &pipeline_layout,
        const std::vector<VkPipelineShaderStageCreateInfo> &pipeline_shaders_stages,
        const VkPipelineRasterizationStateCreateInfo &rasterization_state,
        const VkRenderPass &render_pass,
        const VkPipelineVertexInputStateCreateInfo &vertex_input_state,
        const VkPipelineViewportStateCreateInfo &viewport_state
    );

    void destroy_graphics_pipeline
    (
        VkPipeline &graphics_pipeline,
        const VkDevice &logical_device
    );

    /////////////////////////////////////////////////
    //////////////////// Classes ////////////////////
    /////////////////////////////////////////////////

    /////////////////////////////////
    ///// graphics.pipeline.cpp /////
    /////////////////////////////////

    class graphics_pipeline_handler
    {
        public:
            graphics_pipeline_handler
            (
                const VkPipelineInputAssemblyStateCreateInfo &assembly_input_state,
                const VkPipelineDynamicStateCreateInfo &dynamic_state,
                const VkDevice &logical_device,
                const VkPipelineMultisampleStateCreateInfo &multisampling_state,
                const VkPipelineLayout &pipeline_layout,
                const std::vector<VkPipelineShaderStageCreateInfo> &pipeline_shaders_stages,
                const VkPipelineRasterizationStateCreateInfo &rasterization_state,
                const VkRenderPass &render_pass,
                const VkPipelineVertexInputStateCreateInfo &vertex_input_state,
                const VkPipelineViewportStateCreateInfo &viewport_state
            );

            ~graphics_pipeline_handler();
            VkPipeline get() const;

            // Prevent data duplication.
            graphics_pipeline_handler(const graphics_pipeline_handler&) = delete;
            graphics_pipeline_handler &operator = (const graphics_pipeline_handler&) = delete;
        private:
            VkPipeline graphics_pipeline = VK_NULL_HANDLE;
            VkDevice logical_device = VK_NULL_HANDLE;
    };
}

#endif
