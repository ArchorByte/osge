#include "command.buffer.recorder.hpp"

#include "../vertex/vertex.handler.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <unistd.h>
#include <cstdint>
#include <vector>
#include <array>

// Record the current state of a command buffer for rendering.
void record_command_buffer
(
    const VkCommandBuffer &command_buffer,
    const uint32_t &image_index,
    const VkExtent2D &extent,
    const std::vector<VkFramebuffer> &framebuffers,
    const VkRenderPass &render_pass,
    const VkPipeline &graphics_pipeline,
    const VkViewport &viewport,
    const VkRect2D &scissor,
    const VkBuffer &vertex_buffer,
    const VkBuffer &index_buffer,
    const size_t &frame,
    const VkPipelineLayout &pipeline_layout,
    const std::vector<VkDescriptorSet> descriptor_sets,
    const std::vector<VkImageView> texture_image_views,
    std::vector<uint32_t> indices
)
{
    if (command_buffer == VK_NULL_HANDLE)
    {
        error_log("Failed to render a frame! The command buffer provided (" + force_string(command_buffer) + ") is not valid!");
        return;
    }

    if (framebuffers.size() < 1)
    {
        error_log("Failed to render a frame! No frame buffers were provided!");
        return;
    }

    if (image_index >= framebuffers.size())
    {
        error_log("Failed to render a frame! The image index provided is out of bounds for the frame buffers: " + std::to_string(image_index) + " >= " + std::to_string(framebuffers.size()) + ".");
        return;
    }

    if (render_pass == VK_NULL_HANDLE)
    {
        error_log("Failed to render a frame! The render pass provided (" + force_string(render_pass) + ") is not valid!");
        return;
    }

    if (graphics_pipeline == VK_NULL_HANDLE)
    {
        error_log("Failed to render a frame! The graphics pipeline provided (" + force_string(graphics_pipeline) + ") is not valid!");
        return;
    }

    if (vertex_buffer == VK_NULL_HANDLE)
    {
        error_log("Failed to render a frame! The vertex buffer provided (" + force_string(vertex_buffer) + ") is not valid!");
        return;
    }

    if (index_buffer == VK_NULL_HANDLE)
    {
        error_log("Failed to render a frame! The index buffer provided (" + force_string(index_buffer) + ") is not valid!");
        return;
    }

    if (pipeline_layout == VK_NULL_HANDLE)
    {
        error_log("Failed to render a frame! The pipeline layout provided (" + force_string(pipeline_layout) + ") is not valid!");
        return;
    }

    if (descriptor_sets.size() < 1)
    {
        error_log("Failed to render a frame! No descriptor sets were provided!");
        return;
    }

    if (frame >= descriptor_sets.size())
    {
        error_log("Failed to render a frame! The frame index provided is out of bounds for the descriptor sets: " + std::to_string(frame) + " >= " + std::to_string(descriptor_sets.size()) + ".");
        return;
    }

    if (texture_image_views.size() < 1)
    {
        error_log("Failed to render a frame! No texture image views were provided!");
        return;
    }

    VkCommandBufferBeginInfo begin_info
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
    };

    const VkResult buffer_launch = vkBeginCommandBuffer(command_buffer, &begin_info);

    if (buffer_launch != VK_SUCCESS)
    {
        fatal_error_log("Failed to render a frame! The command buffer start returned error code " + std::to_string(buffer_launch) + ".");
    }

    std::array<VkClearValue, 2> clear_values {};
    clear_values[0].color = {{ 0.0f, 0.0f, 0.0f, 1.0f }};
    clear_values[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo render_pass_begin_info
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = render_pass,
        .framebuffer = framebuffers[image_index],
        .renderArea =
        {
            .offset = { 0, 0 }, // Select the beginning of the rendering area.
            .extent = extent    // Pass the swap chain extent.
        },
        .clearValueCount = static_cast<uint32_t>(clear_values.size()), // Amount of clear values to pass.
        .pClearValues = clear_values.data()
    };

    const VkBuffer vertex_buffers = { vertex_buffer };
    const VkDeviceSize offsets[] = { 0 };

    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline); // Bind the graphics pipeline to the command buffer.
    vkCmdBindVertexBuffers(command_buffer, 0, 1, &vertex_buffers, offsets);                // Bind the vertex buffers to the command buffer.
    vkCmdBindIndexBuffer(command_buffer, index_buffer, 0, VK_INDEX_TYPE_UINT32);           // Bind the index buffer to the command buffer.
    vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0, 1, &descriptor_sets[frame], 0, nullptr); // Bind the descriptor set to the command buffer.

    int targeted_texture = 0;

    // Select the default texture if the targeted texture doesn't exist.
    if (targeted_texture < 0 || targeted_texture > texture_image_views.size())
    {
        error_log("Texture #" + std::to_string(targeted_texture) + " not found!");
        targeted_texture = 0;
    }

    vkCmdPushConstants(command_buffer, pipeline_layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(int), &targeted_texture); // Apply the texture.
    vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE); // Start the render pass for drawing.
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);                                         // Set the viewport.
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);                                           // Set the scissor.
    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);     // Bind the graphics pipeline to the command buffer.
    vkCmdDrawIndexed(command_buffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);       // Make the draw call.
    vkCmdEndRenderPass(command_buffer);                                                        // End the render pass.

    const VkResult buffer_end = vkEndCommandBuffer(command_buffer);

    if (buffer_end != VK_SUCCESS)
    {
        fatal_error_log("Failed to render a frame! The command buffer end returned error code " + std::to_string(buffer_end) + ".");
    }
}
