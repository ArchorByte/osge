#include "vulkan.colors.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

VkPipelineColorBlendStateCreateInfo Colors::create_color_blend_state()
{
    Utils::Logs::log("Creating a color blend state..");

    /*
        blendEnable    / Set color blending status.
        colorWriteMask / Defines which color components will be enabled.
    */
    const VkPipelineColorBlendAttachmentState attachment
    {
        .blendEnable = VK_FALSE,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    /*
        sType           / Defines the type of the structure. Here, we define it as a color blend state create info.
        logicOpEnable   / Defines whether we use logical operations or not.
        attachmentCount / Amount of attachments to pass.
        pAttachments    / Passes the attachments.
    */
    const VkPipelineColorBlendStateCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .attachmentCount = 1,
        .pAttachments = &attachment
    };

    Utils::Logs::log("Color blend state created successfully!");
    return create_info;
}
