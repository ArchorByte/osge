#include "vulkan.pipeline.hpp"

#include "libraries/vulkan/vulkan.h"
#include "../../../utils/utils.hpp"

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a pipeline layout.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Sets push constant range to quickly provide small amount of uniform data to shaders.
        3) Create pipeline layout.

    Parameters:
        - descriptor_set_layout / VkDescriptorSetLayout / Describes the structure of a descriptor set.
        - logical_device        / VkDevice              / Logical device of the Vulkan instance.

    Returns:
        The created pipeline layout.
*/
VkPipelineLayout Pipeline::create_pipeline_layout
(
    const VkDescriptorSetLayout &descriptor_set_layout,
    const VkDevice              &logical_device
)
{
    Utils::Logs::log("Creating pipeline layout.. ", false);

    if (descriptor_set_layout == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Descriptor set layout invalid.");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    /*
        - stateFlags / Defines which shader stages will be able to access a range of push constants.
        - offset     / Defines the beginning of the range.
        - size       / Defines the size of the range.
    */
    const VkPushConstantRange push_constant_range
    {
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .offset = 0,
        .size = sizeof(int)
    };

    /*
        - sType                  / Defines the type of the structure.
        - setLayoutCount         / Defines the amount of descriptor set layouts to pass.
        - pSetLayouts            / Passes the descriptor set layouts.
        - pushConstantRangeCount / Defines the amount of push constant range to pass.
        - pPushConstantRanges    / Passes the push constant ranges.
    */
    const VkPipelineLayoutCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 1,
        .pSetLayouts = &descriptor_set_layout,
        .pushConstantRangeCount = 1,
        .pPushConstantRanges = &push_constant_range
    };

    VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
    const VkResult layout_creation = vkCreatePipelineLayout(logical_device, &create_info, nullptr, &pipeline_layout);

    if (layout_creation != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Creation returned error code -> " + std::to_string(layout_creation) + ".");

    Utils::Logs::log("Done! Memory address -> " + Utils::Text::get_memory_address(pipeline_layout) + ".", true);
    return pipeline_layout;
}



/*
    Destroy a pipeline layout.

    Tasks:
        1) Verify function parameters.
        2) Destroy pipeline layout.
        3) Set object to null.

    Parameters:
        - logical_device  / VkDevice         / Logical device of the Vulkan instance.
        - pipeline_layout / VkPipelineLayout / Pipeline layout to destroy.

    Returns:
        No object returned.
*/
void Pipeline::destroy_pipeline_layout
(
    const VkDevice   &logical_device,
    VkPipelineLayout &pipeline_layout
)
{
    Utils::Logs::log("Destroying " + Utils::Text::get_memory_address(pipeline_layout) + " pipeline layout.. ", false);

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    if (pipeline_layout == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Pipeline layout invalid.", true);
        return;
    }

    vkDestroyPipelineLayout(logical_device, pipeline_layout, nullptr);
    pipeline_layout = VK_NULL_HANDLE;

    Utils::Logs::log("Done!", true);
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Pipeline::pipeline_layout_handler::pipeline_layout_handler
(
    const VkDescriptorSetLayout &descriptor_set_layout,
    const VkDevice              &logical_device
)
    : logical_device(logical_device)
{
    pipeline_layout = create_pipeline_layout(descriptor_set_layout, logical_device);
}

Pipeline::pipeline_layout_handler::~pipeline_layout_handler()
{
    destroy_pipeline_layout(logical_device, pipeline_layout);
}

VkPipelineLayout Pipeline::pipeline_layout_handler::get() const
{
    return pipeline_layout;
}
