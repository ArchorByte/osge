#ifndef VULKAN_PIPELINE_HPP
#define VULKAN_PIPELINE_HPP

#include "libraries/vulkan/vulkan.h"
#include <vector>

namespace Pipeline
{
    ///////////////////////////////////////////////////
    //////////////////// Functions ////////////////////
    ///////////////////////////////////////////////////

    //////////////////////////////
    ///// dynamic.states.cpp /////
    //////////////////////////////

    VkPipelineDynamicStateCreateInfo create_dynamic_states();

    /////////////////////////////////
    ///// graphics.pipeline.cpp /////
    /////////////////////////////////

    VkPipeline create_graphics_pipeline
    (
        const VkPipelineInputAssemblyStateCreateInfo       &assembly_input_state,
        const VkPipelineColorBlendStateCreateInfo          &color_blend_state,
        const VkPipelineDepthStencilStateCreateInfo        &depth_stencil_state,
        const VkPipelineDynamicStateCreateInfo             &dynamic_state,
        const VkDevice                                     &logical_device,
        const VkPipelineMultisampleStateCreateInfo         &multisampling_state,
        const VkPipelineLayout                             &pipeline_layout,
        const std::vector<VkPipelineShaderStageCreateInfo> &pipeline_shader_stages,
        const VkPipelineRasterizationStateCreateInfo       &rasterization_state,
        const VkRenderPass                                 &render_pass,
        const VkPipelineVertexInputStateCreateInfo         &vertex_input_state,
        const VkPipelineViewportStateCreateInfo            &viewport_state
    );

    void destroy_graphics_pipeline
    (
        VkPipeline &graphics_pipeline,
        const VkDevice &logical_device
    );

    /////////////////////////////
    ///// multisampling.cpp /////
    /////////////////////////////

    VkPipelineMultisampleStateCreateInfo create_multisampling_state
    (
        const VkSampleCountFlagBits &samples_count
    );

    ///////////////////////////////
    ///// pipeline.layout.cpp /////
    ///////////////////////////////

    VkPipelineLayout create_pipeline_layout
    (
        const VkDescriptorSetLayout &descriptor_set_layout,
        const VkDevice              &logical_device
    );

    void destroy_pipeline_layout
    (
        const VkDevice   &logical_device,
        VkPipelineLayout &pipeline_layout
    );

    /////////////////////////////
    ///// rasterization.cpp /////
    /////////////////////////////

    VkPipelineRasterizationStateCreateInfo create_rasterization_state();

    ///////////////////////
    ///// scissor.cpp /////
    ///////////////////////

    VkRect2D create_scissor
    (
        const VkExtent2D &extent
    );

    ////////////////////////
    ///// viewport.cpp /////
    ////////////////////////

    VkViewport create_viewport
    (
        const VkExtent2D &extent
    );

    VkPipelineViewportStateCreateInfo create_viewport_state();

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
                const VkPipelineInputAssemblyStateCreateInfo       &assembly_input_state,
                const VkPipelineColorBlendStateCreateInfo          &color_blend_state,
                const VkPipelineDepthStencilStateCreateInfo        &depth_stencil_state,
                const VkPipelineDynamicStateCreateInfo             &dynamic_state,
                const VkDevice                                     &logical_device,
                const VkPipelineMultisampleStateCreateInfo         &multisampling_state,
                const VkPipelineLayout                             &pipeline_layout,
                const std::vector<VkPipelineShaderStageCreateInfo> &pipeline_shader_stages,
                const VkPipelineRasterizationStateCreateInfo       &rasterization_state,
                const VkRenderPass                                 &render_pass,
                const VkPipelineVertexInputStateCreateInfo         &vertex_input_state,
                const VkPipelineViewportStateCreateInfo            &viewport_state
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

    ///////////////////////////////
    ///// pipeline.layout.cpp /////
    ///////////////////////////////

    class pipeline_layout_handler
    {
        public:
            pipeline_layout_handler
            (
                const VkDescriptorSetLayout &descriptor_set_layout,
                const VkDevice              &logical_device
            );

            ~pipeline_layout_handler();
            VkPipelineLayout get() const;

            // Prevent data duplication.
            pipeline_layout_handler(const pipeline_layout_handler&) = delete;
            pipeline_layout_handler &operator = (const pipeline_layout_handler&) = delete;
        private:
            VkDevice logical_device = VK_NULL_HANDLE;
            VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
    };
}

#endif
