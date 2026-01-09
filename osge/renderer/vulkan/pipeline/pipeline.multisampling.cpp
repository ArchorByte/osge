#include "pipeline.multisampling.hpp"

#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>

// Create a multisampling state for a graphics pipeline.
VkPipelineMultisampleStateCreateInfo create_vulkan_multisampling_state
(
    const VkSampleCountFlagBits &samples_count
)
{
    log("Creating a multisampling state..");

    const VkPipelineMultisampleStateCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = samples_count,
        .sampleShadingEnable = VK_TRUE,
        .minSampleShading = .2f,           // Closer to one = better quality but higher resources consumption.
        .alphaToCoverageEnable = VK_FALSE, // Disable the alpha-to-coverage feature.
        .alphaToOneEnable = VK_FALSE       // Disable the alpha-to-one feature.
    };

    log("Multisampling state created successfully!");
    return create_info;
}
