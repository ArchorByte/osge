#include "command.buffer.recorder.hpp"

#include "../vertex/vertex.handler.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <unistd.h>
#include <cstdint>
#include <vector>

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
    const std::vector<VkDescriptorSet> descriptor_sets
)
{
    if (!command_buffer || command_buffer == VK_NULL_HANDLE)
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

    if (!render_pass || render_pass == VK_NULL_HANDLE)
    {
        error_log("Failed to render a frame! The render pass provided (" + force_string(render_pass) + ") is not valid!");
        return;
    }

    if (!graphics_pipeline || graphics_pipeline == VK_NULL_HANDLE)
    {
        error_log("Failed to render a frame! The graphics pipeline provided (" + force_string(graphics_pipeline) + ") is not valid!");
        return;
    }

    if (!vertex_buffer || vertex_buffer == VK_NULL_HANDLE)
    {
        error_log("Failed to render a frame! The vertex buffer provided (" + force_string(vertex_buffer) + ") is not valid!");
        return;
    }

    if (!index_buffer || index_buffer == VK_NULL_HANDLE)
    {
        error_log("Failed to render a frame! The index buffer provided (" + force_string(index_buffer) + ") is not valid!");
        return;
    }

    if (!pipeline_layout || pipeline_layout == VK_NULL_HANDLE)
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

    // Command buffer start info.
    VkCommandBufferBeginInfo begin_info {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = 0;                  // We don't have any special usage flags to pass.
    begin_info.pInheritanceInfo = nullptr; // We don't need to use it.

    // Try to start the command buffer.
    VkResult buffer_launch = vkBeginCommandBuffer(command_buffer, &begin_info);

    if (buffer_launch != VK_SUCCESS)
    {
        fatal_error_log("Failed to render a frame! The command buffer start returned code error " + std::to_string(buffer_launch) + ".");
    }

    // Default black color.
    VkClearValue clear_color = {{{ 0.0f, 0.0f, 0.0f, 1.0f }}};

    // Prepare the info for the render pass start.
    VkRenderPassBeginInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = render_pass;                // Pass the render pass that we are going to use.
    render_pass_info.framebuffer = framebuffers[image_index]; // Select the right frame buffer.
    render_pass_info.renderArea.offset = { 0, 0 };            // Select the rendering area.
    render_pass_info.renderArea.extent = extent;              // Pass the swap chain extent.
    render_pass_info.clearValueCount = 1;                     // Amount of clear values that we are going to use.
    render_pass_info.pClearValues = &clear_color;             // Pass a pointer to our clear color.

    VkBuffer vertex_buffers = { vertex_buffer };
    VkDeviceSize offsets[] = { 0 };

    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline); // Bind the graphics pipeline to the command buffer.
    vkCmdBindVertexBuffers(command_buffer, 0, 1, &vertex_buffers, offsets);                // Bind the vertex buffers to the command buffer.
    vkCmdBindIndexBuffer(command_buffer, index_buffer, 0, VK_INDEX_TYPE_UINT16);           // Bind the index buffer to the command buffer.
    vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0, 1, &descriptor_sets[frame], 0, nullptr); // Bind the descriptor set selected by the frame to the command buffer.

    vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);   // Start the render pass for the drawing.
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);                                     // Set the viewport (we do that each time because it's dynamic).
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);                                       // Set the scissor (same here).
    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline); // Bind the graphics pipeline to the command buffer.
    vkCmdDrawIndexed(command_buffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);   // Make the draw call.
    vkCmdEndRenderPass(command_buffer);                                                    // End the render pass.
    VkResult buffer_end = vkEndCommandBuffer(command_buffer);                              // Try to end the command buffer.

    if (buffer_end != VK_SUCCESS)
    {
        fatal_error_log("Failed to render a frame! The command buffer end returned error code " + std::to_string(buffer_end) + ".");
    }
}
