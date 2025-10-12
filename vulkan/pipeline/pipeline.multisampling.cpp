#include "pipeline.multisampling.hpp"

#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>

// Create a multisampling state for a graphics pipeline.
VkPipelineMultisampleStateCreateInfo create_vulkan_multisampling_state()
{
    log("Creating a multisampling state..");

    // Create info for the multisampling state.
    VkPipelineMultisampleStateCreateInfo multisample_state_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT, // Disable multisampling.
        .sampleShadingEnable = VK_FALSE,               // Disable sample shading.
        .alphaToCoverageEnable = VK_FALSE,             // Disable the alpha-to-coverage feature.
        .alphaToOneEnable = VK_FALSE                   // Disable the alpha-to-one feature.
    };

    log("Multisampling state created successfully!");
    return multisample_state_info;
}
