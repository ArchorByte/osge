#include "pipeline.multisampling.hpp"

#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>

// Create a multisampling state for a graphics pipeline.
VkPipelineMultisampleStateCreateInfo create_vulkan_multisampling_state()
{
    log("Creating a multisampling state..");

    // Create info for the multisampling state.
    VkPipelineMultisampleStateCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    info.sampleShadingEnable = VK_FALSE;               // Disable sample shading.
    info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; // Disable multisampling.
    info.minSampleShading = 1.0f;                      // Useless as today as the sample shading is turned off but useful for later.
    info.pSampleMask = nullptr;                        // No sample mask used.
    info.alphaToCoverageEnable = VK_FALSE;             // Disable the "alpha-to-coverage" feature.
    info.alphaToOneEnable = VK_FALSE;                  // Disable the "alpha-to-one" feature.

    log("Multisampling state created successfully!");
    return info;
}
