#include "pipeline.rasterization.hpp"

#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>

// Create a rasterization state for a graphics pipeline.
VkPipelineRasterizationStateCreateInfo create_vulkan_rasterization_state()
{
    log("Creating a rasterization state..");

    // Create info for the rasterization state
    VkPipelineRasterizationStateCreateInfo rasterization_create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,                 // Disable depth clamp.
        .rasterizerDiscardEnable = VK_FALSE,          // Disable the rasterization discard.
        .polygonMode = VK_POLYGON_MODE_FILL,          // Fill the polygons with pixels instead of letting them empty.
        .cullMode = VK_CULL_MODE_BACK_BIT,            // Disable the rendering for not visible parts of the triangles.
        .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,                  // Disable depth bias.
    };

    log("Rasterization state created successfully!");
    return rasterization_create_info;
}
