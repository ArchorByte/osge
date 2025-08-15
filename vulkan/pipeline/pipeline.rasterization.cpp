#include "pipeline.rasterization.hpp"

#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>

// Create a rasterization state for a graphics pipeline.
VkPipelineRasterizationStateCreateInfo create_vulkan_rasterization_state()
{
    log("Creating a rasterization state..");

    // Create info for the rasterization state
    VkPipelineRasterizationStateCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    info.depthClampEnable = VK_FALSE;                 // Disable depth clamp.
    info.rasterizerDiscardEnable = VK_FALSE;          // Disable rasterization discard.
    info.polygonMode = VK_POLYGON_MODE_FILL;          // Fill the polygons with pixels instead of letting them empty.
    info.lineWidth = 1.0f;                            // Idk but it's in the docs.
    info.depthBiasEnable = VK_FALSE;                  // Disable depth bias.
    info.depthBiasConstantFactor = 0.0f;              // Disable the depth bias constant refactor as we don't need it.
    info.depthBiasClamp = 0.0f;                       // Disable the depth bias clamp as we don't need it.
    info.depthBiasSlopeFactor = 0.0f;                 // Disable the depth bias slope factor as we don't need it as well.
    info.cullMode = VK_CULL_MODE_BACK_BIT;            // Disable the rendering for not visible parts of the triangles.
    info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

    log("Rasterization state created successfully!");
    return info;
}
