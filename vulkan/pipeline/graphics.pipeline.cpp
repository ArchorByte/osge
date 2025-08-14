#include "graphics_pipeline.hpp"

#include "../../logs/terminal.hpp"
#include "../../tools/text_format.hpp"

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

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Graphics pipeline creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (pipeline_shaders_stages.size() < 1)
    {
        fatal_error_log("Graphics pipeline creation failed! No pipeline shaders stages were provided!");
    }

    if (!pipeline_layout || pipeline_layout == VK_NULL_HANDLE)
    {
        fatal_error_log("Graphics pipeline creation failed! The pipeline layout provided (" + force_string(pipeline_layout) + ") is not valid!");
    }

    if (!render_pass || render_pass == VK_NULL_HANDLE)
    {
        fatal_error_log("Graphics pipeline creation failed! The render pass provided (" + force_string(render_pass) + ") is not valid!");
    }

    // Attachment for the color blend state.
    VkPipelineColorBlendAttachmentState color_blend_attachment_state {};
    color_blend_attachment_state.blendEnable = VK_FALSE;                     // Disable the blend.
    color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Not used as the blend is disabled.
    color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Same here.
    color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;             // Same here.
    color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Same here.
    color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Same here.
    color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;             // Same here.
    color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT; // Enable RGBA.

    // Create info for the color blend state.
    VkPipelineColorBlendStateCreateInfo color_blend_create_info {};
    color_blend_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_create_info.logicOpEnable = VK_FALSE;                     // Disable logical operations.
    color_blend_create_info.logicOp = VK_LOGIC_OP_COPY;                   // Not used as the logical operations are disabled.
    color_blend_create_info.attachmentCount = 1;                          // Amount of attachment to pass.
    color_blend_create_info.pAttachments = &color_blend_attachment_state; // Pass our color blend attachment state.
    color_blend_create_info.blendConstants[0] = 0.0f;                     // Some blend constants.
    color_blend_create_info.blendConstants[1] = 0.0f;
    color_blend_create_info.blendConstants[2] = 0.0f;
    color_blend_create_info.blendConstants[3] = 0.0f;

    // Create info for the graphics pipeline.
    VkGraphicsPipelineCreateInfo pipeline_create_info {};
    pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_create_info.stageCount = static_cast<uint32_t>(pipeline_shaders_stages.size()); // Amount of shaders stages to pass.
    pipeline_create_info.pStages = pipeline_shaders_stages.data();                           // Pass the shaders stages.
    pipeline_create_info.pVertexInputState = &vertex_input_state;                            // Pass the vertex input state.
    pipeline_create_info.pInputAssemblyState = &assembly_input_state;                        // Pass the assembly input state.
    pipeline_create_info.pViewportState = &viewport_state;                                   // Pass the viewport state.
    pipeline_create_info.pRasterizationState = &rasterization_state;                         // Pass the rasterization state.
    pipeline_create_info.pMultisampleState = &multisampling_state;                           // Pass the multisampling state.
    pipeline_create_info.pDepthStencilState = nullptr;                                       // No depth and stencil used.
    pipeline_create_info.pColorBlendState = &color_blend_create_info;                        // Pass the color blend state create info.
    pipeline_create_info.pDynamicState = &dynamic_state;                                     // Pass the dynamic state.
    pipeline_create_info.layout = pipeline_layout;                                           // Pass the pipeline layout.
    pipeline_create_info.renderPass = render_pass;                                           // Pass the render pass.
    pipeline_create_info.subpass = 0;                                                        // No subpass passed.
    pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;                                // Not used.
    pipeline_create_info.basePipelineIndex = -1;                                             // Not used.

    VkPipeline graphics_pipeline = VK_NULL_HANDLE;
    VkResult pipeline_creation = vkCreateGraphicsPipelines(logical_device, VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &graphics_pipeline); // Try to create the graphics pipeline.

    if (pipeline_creation != VK_SUCCESS)
    {
        fatal_error_log("Graphics pipeline creation returned error code " + std::to_string(pipeline_creation) + ".");
    }

    if (!graphics_pipeline || graphics_pipeline == VK_NULL_HANDLE)
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

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        error_log("Graphics pipeline destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (!graphics_pipeline || graphics_pipeline == VK_NULL_HANDLE)
    {
        error_log("Graphics pipeline destruction failed! The graphics pipeline provided (" + force_string(graphics_pipeline) + ") is not valid!");
    }

    // Destroy the graphics pipeline and dispose of the address.
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
