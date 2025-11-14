#include "draw.frames.hpp"

#include "../commands/command.buffer.recorder.hpp"
#include "../uniform/uniform.buffer.update.hpp"
#include "../uniform/uniform.buffers.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <unistd.h>
#include <vector>
#include <string>

// Render, draw and present a frame.
std::string draw_frame
(
    const VkDevice &logical_device,
    const std::vector<VkFence> &fences,
    const VkSwapchainKHR &swapchain,
    const std::vector<VkSemaphore> &image_available_semaphores,
    const std::vector<VkSemaphore> &render_finished_semaphores,
    const std::vector<VkCommandBuffer> &command_buffers,
    const VkExtent2D &extent,
    const std::vector<VkFramebuffer> &framebuffers,
    const VkRenderPass &render_pass,
    const VkPipeline &graphics_pipeline,
    const VkViewport &viewport,
    const VkRect2D &scissor,
    const VkQueue &graphics_queue,
    const VkQueue &present_queue,
    const size_t &frame,
    const VkBuffer &vertex_buffer,
    const VkBuffer &index_buffer,
    const std::vector<UniformBufferInfo> &uniform_buffers,
    const VkPipelineLayout &pipeline_layout,
    const std::vector<VkDescriptorSet> descriptor_sets,
    const std::vector<VkImageView> texture_image_views,
    std::vector<uint32_t> indices
)
{
    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Failed to draw a frame! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return "failed";
    }

    if (fences.size() < 1)
    {
        error_log("Failed to draw a frame! No fences were provided!");
        return "failed";
    }

    if (swapchain == VK_NULL_HANDLE)
    {
        error_log("Failed to draw a frame! The swap chain provided (" + force_string(swapchain) + ") is not valid!");
        return "failed";
    }

    if (image_available_semaphores.size() < 1)
    {
        error_log("Failed to draw a frame! No image available semaphores were provided!");
        return "failed";
    }

    if (render_finished_semaphores.size() < 1)
    {
        error_log("Failed to draw a frame! No render finished semaphores were provided!");
        return "failed";
    }

    if (command_buffers.size() < 1)
    {
        error_log("Failed to draw a frame! No command buffers were provided!");
        return "failed";
    }

    if (framebuffers.size() < 1)
    {
        error_log("Failed to draw a frame! No frame buffers were provided!");
        return "failed";
    }

    if (render_pass == VK_NULL_HANDLE)
    {
        error_log("Failed to draw a frame! The render pass provided (" + force_string(render_pass) + ") is not valid!");
        return "failed";
    }

    if (graphics_pipeline == VK_NULL_HANDLE)
    {
        error_log("Failed to draw a frame! The graphics pipeline provided (" + force_string(graphics_pipeline) + ") is not valid!");
        return "failed";
    }

    if (graphics_queue == VK_NULL_HANDLE)
    {
        error_log("Failed to draw a frame! The graphics queue provided (" + force_string(graphics_queue) + ") is not valid!");
        return "failed";
    }

    if (present_queue == VK_NULL_HANDLE)
    {
        error_log("Failed to draw a frame! The present queue provided (" + force_string(present_queue) + ") is not valid!");
        return "failed";
    }

    if (frame >= fences.size())
    {
        error_log("Failed to draw a frame! The frame index is out of bounds for the fences: " + std::to_string(frame) + " >= " + std::to_string(fences.size()) + ".");
        return "failed";
    }

    if (frame >= image_available_semaphores.size())
    {
        error_log("Failed to draw a frame! The frame index is out of bound for the image available semaphores: " + std::to_string(frame) + " >= " + std::to_string(image_available_semaphores.size()) + ".");
        return "failed";
    }

    if (frame >= command_buffers.size())
    {
        error_log("Failed to draw a frame! The frame index is out of bound for the command buffers: " + std::to_string(frame) + " >= " + std::to_string(command_buffers.size()) + ".");
        return "failed";
    }

    if (vertex_buffer == VK_NULL_HANDLE)
    {
        error_log("Failed to draw a frame! The vertex buffer provided (" + force_string(vertex_buffer) + ") is not valid!");
        return "failed";
    }

    if (index_buffer == VK_NULL_HANDLE)
    {
        error_log("Failed to draw a frame! The index buffer provided (" + force_string(index_buffer) + ") is not valid!");
        return "failed";
    }

    if (uniform_buffers.size() < 1)
    {
        error_log("Failed to draw a frame! No uniform buffers were provided!");
        return "failed";
    }

    if (pipeline_layout == VK_NULL_HANDLE)
    {
        error_log("Failed to draw a frame! The pipeline layout provided (" + force_string(pipeline_layout) + ") is not valid!");
        return "failed";
    }

    if (descriptor_sets.size() < 1)
    {
        error_log("Failed to draw a frame! No descriptor sets were provided!");
        return "failed";
    }

    if (texture_image_views.size() < 1)
    {
        error_log("Failed to draw a frame! No texture image views were provided!");
        return "failed";
    }

    // Wait for the fence to be available.
    vkWaitForFences(logical_device, 1, &fences[frame], VK_TRUE, UINT64_MAX);

    // Try to acquire the next image to display on screen.
    uint32_t image_index;
    VkResult acquire_image = vkAcquireNextImageKHR(logical_device, swapchain, UINT64_MAX, image_available_semaphores[frame], VK_NULL_HANDLE, &image_index);

    if (acquire_image == VK_ERROR_OUT_OF_DATE_KHR)
    {
        error_log("Failed to draw a frame! The swap chain is outdated.");
        return "recreate";
    }

    if (acquire_image == VK_SUBOPTIMAL_KHR)
    {
        error_log("Failed to draw a frame! The swap chain is suboptimal.");
        return "recreate";
    }

    if (acquire_image != VK_SUCCESS)
    {
        error_log("Failed to draw a frame! Next image acquirement returned error code " + std::to_string(acquire_image) + ".");
        return "failed";
    }

    if (image_index >= render_finished_semaphores.size())
    {
        error_log("Failed to draw a frame! The image index is out of bounds for the semaphores: " + std::to_string(image_index) + " >= " + std::to_string(render_finished_semaphores.size()) + ".");
        return "failed";
    }

    vkResetFences(logical_device, 1, &fences[frame]); // Reset the fence.
    vkResetCommandBuffer(command_buffers[frame], 0);  // Reset the command buffer.

    // Record the command buffer state.
    record_command_buffer(command_buffers[frame], image_index, extent, framebuffers, render_pass, graphics_pipeline, viewport, scissor, vertex_buffer, index_buffer, frame, pipeline_layout, descriptor_sets, texture_image_views, indices);
    update_uniform_buffer(frame, extent, uniform_buffers[frame].data); // Update the uniform buffer data.

    const VkSemaphore wait_semaphores[] = { image_available_semaphores[frame] };                  // Semaphores to wait on, before we start the command buffer execution.
    const VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT }; // Define at what stage in the rendering process, we start to wait.
    const VkSemaphore signal_semaphores[] = { render_finished_semaphores[image_index] };          // Semaphores to signal after the rendering has finished.

    VkSubmitInfo submit_info
    {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,                    // Amount of wait semaphores to pass.
        .pWaitSemaphores = wait_semaphores,         // Pass the wait semaphores.
        .pWaitDstStageMask = wait_stages,           // Pass the wait stages.
        .commandBufferCount = 1,                    // Amount of command buffer to pass.
        .pCommandBuffers = &command_buffers[frame], // Pass the command buffer.
        .signalSemaphoreCount = 1,                  // Amount of signal semaphores to pass.
        .pSignalSemaphores = signal_semaphores      // Pass the signal semaphores.
    };

    // Try to submit the frame to the graphics queue.
    const VkResult queue_submit = vkQueueSubmit(graphics_queue, 1, &submit_info, fences[frame]);

    if (queue_submit != VK_SUCCESS)
    {
        error_log("Frame submit to graphics queue returned error code: " + std::to_string(queue_submit) + ".");
        return "failed";
    }

    VkPresentInfoKHR present_info
    {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,              // Amount of wait semaphores to pass.
        .pWaitSemaphores = signal_semaphores, // Pass the signal semaphores as wait semaphores.
        .swapchainCount = 1,                  // Amount of swap chains to pass.
        .pSwapchains = &swapchain,            // Pass the swap chain.
        .pImageIndices = &image_index         // Pass the index of the image to present.
    };

    const VkResult present_result = vkQueuePresentKHR(present_queue, &present_info);

    if (present_result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        error_log("Failed to draw a frame! The swap chain is outdated.");
        return "recreate";
    }

    if (present_result != VK_SUCCESS)
    {
        error_log("Failed to draw a frame! Frame presentation returned error code " + std::to_string(present_result) + ".");
        return "failed";
    }

    return "success";
}
