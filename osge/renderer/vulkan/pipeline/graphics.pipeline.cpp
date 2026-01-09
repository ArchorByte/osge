#include "graphics.pipeline.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a graphics pipeline.
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
)
{
    log("Creating a graphics pipeline..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Graphics pipeline creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (pipeline_shaders_stages.size() < 1)
    {
        fatal_error_log("Graphics pipeline creation failed! No pipeline shaders stages were provided!");
    }

    if (pipeline_layout == VK_NULL_HANDLE)
    {
        fatal_error_log("Graphics pipeline creation failed! The pipeline layout provided (" + force_string(pipeline_layout) + ") is not valid!");
    }

    if (render_pass == VK_NULL_HANDLE)
    {
        fatal_error_log("Graphics pipeline creation failed! The render pass provided (" + force_string(render_pass) + ") is not valid!");
    }

    // Attachment for the color blend state.
    VkPipelineColorBlendAttachmentState color_blend_attachment_state
    {
        .blendEnable = VK_FALSE, // Disable the blend.
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    // Create info for the color blend state.
    const VkPipelineColorBlendStateCreateInfo color_blend_create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE, // Disable logical operations.
        .attachmentCount = 1,      // Amount of attachments to pass.
        .pAttachments = &color_blend_attachment_state
    };

    VkPipelineDepthStencilStateCreateInfo depth_stencil
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depthTestEnable = VK_TRUE,
        .depthWriteEnable = VK_TRUE,
        .depthCompareOp = VK_COMPARE_OP_LESS,
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable = VK_FALSE
    };

    const VkGraphicsPipelineCreateInfo pipeline_create_info
    {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = static_cast<uint32_t>(pipeline_shaders_stages.size()), // Amount of shader stages to pass.
        .pStages = pipeline_shaders_stages.data(),
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
    {
        fatal_error_log("Graphics pipeline creation returned error code " + std::to_string(pipeline_creation) + ".");
    }

    if (graphics_pipeline == VK_NULL_HANDLE)
    {
        fatal_error_log("Graphics pipeline creation output \"" + force_string(graphics_pipeline) + "\" is not valid!");
    }

    log("Graphics pipeline " + force_string(graphics_pipeline) + " created successfully!");
    return graphics_pipeline;
}

// Destroy a graphics pipeline.
void destroy_vulkan_graphics_pipeline
(
    const VkDevice &logical_device,
    VkPipeline &graphics_pipeline
)
{
    log("Destroying the " + force_string(graphics_pipeline) + " graphics pipeline..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Graphics pipeline destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (graphics_pipeline == VK_NULL_HANDLE)
    {
        error_log("Graphics pipeline destruction failed! The graphics pipeline provided (" + force_string(graphics_pipeline) + ") is not valid!");
        return;
    }

    vkDestroyPipeline(logical_device, graphics_pipeline, nullptr);
    graphics_pipeline = VK_NULL_HANDLE;

    log("Graphics pipeline destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_GraphicsPipeline::Vulkan_GraphicsPipeline
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
) : logical_device(logical_device)
{
    graphics_pipeline = create_vulkan_graphics_pipeline(logical_device, pipeline_shaders_stages, vertex_input_state, assembly_input_state, viewport_state, rasterization_state, multisampling_state, pipeline_layout, render_pass, dynamic_state);
}

// Destructor.
Vulkan_GraphicsPipeline::~Vulkan_GraphicsPipeline()
{
    destroy_vulkan_graphics_pipeline(logical_device, graphics_pipeline);
}

VkPipeline Vulkan_GraphicsPipeline::get() const
{
    return graphics_pipeline;
}
