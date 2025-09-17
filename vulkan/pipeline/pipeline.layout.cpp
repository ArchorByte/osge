#include "pipeline.layout.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a pipeline layout for a graphics pipeline.
VkPipelineLayout create_vulkan_pipeline_layout
(
    const VkDevice &logical_device,
    const VkDescriptorSetLayout &descriptor_set_layout
)
{
    log("Creating a pipeline layout..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Pipeline layout creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (!descriptor_set_layout || descriptor_set_layout == VK_NULL_HANDLE)
    {
        fatal_error_log("Pipeline layout creation failed! The descriptor set layout provided (" + force_string(descriptor_set_layout) + ") is not valid!");
    }

    // Push constant range for texture selection.
    VkPushConstantRange push_constant_range{};
    push_constant_range.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    push_constant_range.offset = 0;
    push_constant_range.size = sizeof(int);

    // Create info for the pipeline layout.
    VkPipelineLayoutCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.setLayoutCount = 1;                   // Amount of layouts to enable.
    info.pSetLayouts = &descriptor_set_layout; // Pass the descriptor set layout.
    info.pushConstantRangeCount = 1;           // Amount of constant range to pass.
    info.pPushConstantRanges = &push_constant_range;

    VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
    VkResult layout_creation = vkCreatePipelineLayout(logical_device, &info, nullptr, &pipeline_layout); // Try to create the layout.

    if (layout_creation != VK_SUCCESS)
    {
        fatal_error_log("Pipeline layout creation returned error code " + std::to_string(layout_creation) + ".");
    }

    if (!pipeline_layout || pipeline_layout == VK_NULL_HANDLE)
    {
        fatal_error_log("Pipeline layout creation output \"" + force_string(pipeline_layout) + "\" is not valid!");
    }

    log("Pipeline layout " + force_string(pipeline_layout) + " created successfully!");
    return pipeline_layout;
}

// Destroy a pipeline layout.
void destroy_vulkan_pipeline_layout
(
    const VkDevice &logical_device,
    VkPipelineLayout &pipeline_layout
)
{
    log("Destroying the " + force_string(pipeline_layout) + " pipeline layout..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        error_log("Pipeline layout destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (!pipeline_layout || pipeline_layout == VK_NULL_HANDLE)
    {
        error_log("Pipeline layout destruction failed! The pipeline layout provided (" + force_string(pipeline_layout) + ") is not valid!");
        return;
    }

    // Destroy the pipeline layout and dispose of the address.
    vkDestroyPipelineLayout(logical_device, pipeline_layout, nullptr);
    pipeline_layout = VK_NULL_HANDLE;

    log("Pipeline layout destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_PipelineLayout::Vulkan_PipelineLayout
(
    const VkDevice &logical_device,
    const VkDescriptorSetLayout &descriptor_set_layout
) : logical_device(logical_device)
{
    pipeline_layout = create_vulkan_pipeline_layout(logical_device, descriptor_set_layout);
}

// Destructor.
Vulkan_PipelineLayout::~Vulkan_PipelineLayout()
{
    destroy_vulkan_pipeline_layout(logical_device, pipeline_layout);
}

VkPipelineLayout Vulkan_PipelineLayout::get() const
{
    return pipeline_layout;
}
