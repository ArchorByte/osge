#include "vulkan.pipeline.hpp"

#include "../../../utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"

/*
    Create a multisample state for a pipeline.

    Tasks:
        1) Make create info.
        2) Return it.

    Parameters:
        - samples_count / VkSampleCountFlagBits / Amount of samples to use for multisampling.

    Returns:
        The created pipeline multisample state create info.
*/
VkPipelineMultisampleStateCreateInfo create_multisampling_state
(
    const VkSampleCountFlagBits &samples_count
)
{
    Utils::Logs::log("Creating multisampling state.. ", false);

    /*
        - sType                 / Defines the type of the structure.
        - rasterizationSamples  / Amount of samples to use for multisampling.
        - sampleShadingEnable   / Improves quality of images using multisample anti-aliasing.
        - minSampleShading      / Defines the minimum fraction of sample shading. Closer to 1, better quality but greater resource usage.
        - alphaToCoverageEnable / Defines whether we use alpha component to make temporary coverage value or not.
        - alphaToOneEnable      / Defines whether alpha component of fragment's first color is replaced with one or not.
    */
    const VkPipelineMultisampleStateCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = samples_count,
        .sampleShadingEnable = VK_TRUE,
        .minSampleShading = .2f,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE
    };

    Utils::Logs::log("Done!", true);
    return create_info;
}
