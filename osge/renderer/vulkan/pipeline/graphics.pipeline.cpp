#include "vulkan.pipeline.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a graphics pipeline.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the parameters.
        2) Create graphics pipeline.

    Parameters:
        - assembly_input_state    / VkPipelineInputAssemblyStateCreateInfo  / Determines input assembly behavior for vertex shading.
        - color_blend_state       / VkPipelineColorBlendStateCreateInfo     / Defines color blend state for color attachments during rendering.
        - depth_stencil_state     / VkPipelineDepthStencilStateCreateInfo   / Defines depth/stencil state for depth/stencil attachments during rendering.
        - dynamic_state           / VkPipelineDynamicStateCreateInfo        / Defines which properties of the pipeline will be dynamic.
        - logical_device          / VkDevice                                / Logical device of the Vulkan instance.
        - multisampling_state     / VkPipelineMultisampleStateCreateInfo    / Defines multisample state when rasterization is enabled.
        - pipeline_layout         / VkPipelineLayout                        / Describes binding locations used by the pipeline and descriptor sets.
        - pipeline_shader_stages  / vector<VkPipelineShaderStageCreateInfo> / List of shader stages for the pipeline.
        - rasterization_state     / VkPipelineRasterizationStateCreateInfo  / Defines the state of rasterization.
        - render_pass             / VkRenderPass                            / Organizes rendering tasks.
        - vertex_input_state      / VkPipelineVertexInputStateCreateInfo    / Defines how vertex data shall be handled in vertex shaders.
        - viewport_state          / VkPipelineViewportStateCreateInfo       / Defines viewport state used when rasterization is enabled.

    Returns:
        The created graphics pipeline.
*/
VkPipeline Pipeline::create_graphics_pipeline
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
)
{
    Utils::Logs::log("Creating graphics pipeline.. ", false);

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (pipeline_shader_stages.size() < 1)
        Utils::Logs::crash_log("Failed! No pipeline shaders stages provided.");

    if (pipeline_layout == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Pipeline layout invalid.");

    if (render_pass == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Render pass invalid.");

    /*
        - sType               / Defines the type of the structure.
        - stageCount          / Amount of shader stages to pass.
        - pStages             / Pass the shader stages.
        - pVertexInputState   / Defines how vertex data shall be handled in vertex shaders.
        - pInputAssemblyState / Determines input assembly behavior for vertex shading.
        - pViewportState      / Defines viewport state used when rasterization is enabled.
        - pRasterizationState / Defines the state of rasterization.
        - pMultisampleState   / Defines multisample state when rasterization is enabled.
        - pDepthStencilState  / Defines depth/stencil state for depth/stencil attachments during rendering.
        - pColorBlendState    / Defines color blend state for color attachments during rendering.
        - pDynamicState       / Defines which properties of the pipeline will be dynamic.
        - layout              / Describes binding locations used by the pipeline and descriptor sets.
        - renderPass          / Organizes rendering tasks.
    */
    const VkGraphicsPipelineCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = static_cast<uint32_t>(pipeline_shader_stages.size()),
        .pStages = pipeline_shader_stages.data(),
        .pVertexInputState = &vertex_input_state,
        .pInputAssemblyState = &assembly_input_state,
        .pViewportState = &viewport_state,
        .pRasterizationState = &rasterization_state,
        .pMultisampleState = &multisampling_state,
        .pDepthStencilState = &depth_stencil_state,
        .pColorBlendState = &color_blend_state,
        .pDynamicState = &dynamic_state,
        .layout = pipeline_layout,
        .renderPass = render_pass
    };

    VkPipeline graphics_pipeline = VK_NULL_HANDLE;
    const VkResult pipeline_creation = vkCreateGraphicsPipelines(logical_device, VK_NULL_HANDLE, 1, &create_info, nullptr, &graphics_pipeline);

    if (pipeline_creation != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Creation returned error code -> " + std::to_string(pipeline_creation) + ".");

    Utils::Logs::log("Done! Memory address -> " + Utils::Text::get_memory_address(graphics_pipeline) + ".", true);
    return graphics_pipeline;
}



/*
    Destroy a graphics pipeline.

    Tasks:
        1) Verify function parameters.
        2) Destroy graphics pipeline.
        3) Set object to null.

    Parameters:
        - graphics_pipeline / VkPipeline / Graphics pipeline to destroy.
        - logical_device    / VkDevice   / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Pipeline::destroy_graphics_pipeline
(
    VkPipeline &graphics_pipeline,
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Destroying " + Utils::Text::get_memory_address(graphics_pipeline) + " graphics pipeline.. ", false);

    if (graphics_pipeline == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Graphics pipeline invalid.", true);
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    vkDestroyPipeline(logical_device, graphics_pipeline, nullptr);
    graphics_pipeline = VK_NULL_HANDLE;

    Utils::Logs::log("Done!", true);
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Pipeline::graphics_pipeline_handler::graphics_pipeline_handler
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
)
    : logical_device(logical_device)
{
    graphics_pipeline = create_graphics_pipeline(assembly_input_state, color_blend_state, depth_stencil_state, dynamic_state, logical_device, multisampling_state, pipeline_layout, pipeline_shader_stages, rasterization_state, render_pass, vertex_input_state, viewport_state);
}

Pipeline::graphics_pipeline_handler::~graphics_pipeline_handler()
{
    destroy_graphics_pipeline(graphics_pipeline, logical_device);
}

VkPipeline Pipeline::graphics_pipeline_handler::get() const
{
    return graphics_pipeline;
}
