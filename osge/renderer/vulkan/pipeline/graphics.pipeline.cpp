#include "vulkan.pipeline.hpp"
#include "osge/utils/utils.hpp"
#include <libraries/vulkan/vulkan.h>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a graphics pipeline.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the parameters.

    Parameters:
        - assembly_input_state    / VkPipelineInputAssemblyStateCreateInfo  / 
        - dynamic_state           / VkPipelineDynamicStateCreateInfo        / 
        - logical_device          / VkDevice                                / Logical device of the Vulkan instance.
        - multisampling_state     / VkPipelineMultisampleStateCreateInfo    / 
        - pipeline_layout         / VkPipelineLayout                        / Pipeline layout of the Vulkan instance.
        - pipeline_shader_stages  / vector<VkPipelineShaderStageCreateInfo> / Loaded shaders.
        - rasterization_state     / VkPipelineRasterizationStateCreateInfo  / 
        - render_pass             / VkRenderPass                            / Render pass of the Vulkan instance.
        - vertex_input_state      / VkPipelineVertexInputStateCreateInfo    / 
        - viewport_state          / VkPipelineViewportStateCreateInfo       / 

    Returns:
        The created graphics pipeline.
*/
VkPipeline Pipeline::create_graphics_pipeline
(
    const VkPipelineInputAssemblyStateCreateInfo &assembly_input_state,
    const VkPipelineDynamicStateCreateInfo &dynamic_state,
    const VkDevice &logical_device,
    const VkPipelineMultisampleStateCreateInfo &multisampling_state,
    const VkPipelineLayout &pipeline_layout,
    const std::vector<VkPipelineShaderStageCreateInfo> &pipeline_shader_stages,
    const VkPipelineRasterizationStateCreateInfo &rasterization_state,
    const VkRenderPass &render_pass,
    const VkPipelineVertexInputStateCreateInfo &vertex_input_state,
    const VkPipelineViewportStateCreateInfo &viewport_state
)
{
    Utils::Logs::log("Creating a graphics pipeline..");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Graphics pipeline creation failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    if (pipeline_shader_stages.size() < 1)
        Utils::Logs::crash_error_log("Graphics pipeline creation failed! No pipeline shaders stages were provided!");

    if (pipeline_layout == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Graphics pipeline creation failed! The pipeline layout provided (" + Utils::Text::get_memory_address(pipeline_layout) + ") is not valid!");

    if (render_pass == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Graphics pipeline creation failed! The render pass provided (" + Utils::Text::get_memory_address(render_pass) + ") is not valid!");

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
        .pDepthStencilState = &depth_stencil,
        .pColorBlendState = &color_blend_create_info,
        .pDynamicState = &dynamic_state,
        .layout = pipeline_layout,
        .renderPass = render_pass
    };

    VkPipeline graphics_pipeline = VK_NULL_HANDLE;
    const VkResult pipeline_creation = vkCreateGraphicsPipelines(logical_device, VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &graphics_pipeline);

    if (pipeline_creation != VK_SUCCESS)
        Utils::Logs::crash_error_log("Graphics pipeline creation returned error code " + std::to_string(pipeline_creation) + ".");

    Utils::Logs::log("Graphics pipeline " + Utils::Text::get_memory_address(graphics_pipeline) + " created successfully!");
    return graphics_pipeline;
}



/*
    Cleanly destroy a graphics pipeline.

    Tasks:
        1) Verify the parameters.
        2) Destroy the graphics pipeline.
        3) Get rid of the object memory address.

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
    Utils::Logs::log("Destroying the " + Utils::Text::get_memory_address(graphics_pipeline) + " graphics pipeline..");

    if (graphics_pipeline == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Graphics pipeline destruction failed! The graphics pipeline provided (" + Utils::Text::get_memory_address(graphics_pipeline) + ") is not valid!");
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Graphics pipeline destruction failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");
        return;
    }

    vkDestroyPipeline(logical_device, graphics_pipeline, nullptr);
    graphics_pipeline = VK_NULL_HANDLE;

    Utils::Logs::log("Graphics pipeline destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Pipeline::graphics_pipeline_handler::graphics_pipeline_handler
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
)
    : logical_device(logical_device)
{
    graphics_pipeline = create_graphics_pipeline(assembly_input_state, dynamic_state, logical_device, multisampling_state, pipeline_layout, pipeline_shaders_stages, rasterization_state, render_pass, vertex_input_state, viewport_state);
}

Pipeline::graphics_pipeline_handler::~graphics_pipeline_handler()
{
    destroy_graphics_pipeline(graphics_pipeline, logical_device);
}

VkPipeline Pipeline::graphics_pipeline_handler::get() const
{
    return graphics_pipeline;
}
