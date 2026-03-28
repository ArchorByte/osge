#include "vulkan.render.hpp"

#include "../buffers/vulkan.buffers.hpp"
#include "../../../utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"

#include <string>
#include <unistd.h>
#include <vector>

/*
    Render, draw and present frame.

    Tasks:
        1) Verify function parameters.
        2) Wait for fences activity to end.
        3) Acquire an image in image available semaphores.
        4) Reset fence and command buffer to use it.
        5) Record command buffer:
            - Start render pass.
            - Bind rendering objects altogether.
            - Set viewport and scissor.
            - Make draw call.
            - End render pass.
        6) Update uniform buffer.
        7) Submit command buffer to graphics queue.
            - Wait on image available semaphore.
            - Send signal to render finished semaphore when rendering is done.
        8) Present rendered image.

    Parameters:
        - command_buffers            / vector<VkCommandBuffer>   / Records and submits to the GPU.
        - descriptor_sets            / vector<VkDescriptorSet>   / Regroups data together.
        - extent                     / VkExtent2D                / Resolution of the swap chain.
        - fences                     / vector<VkFence>           / Works for GPU and CPU synchronisation.
        - frame                      / size_t                    / Current frame that we are going to work on.
        - framebuffers               / vector<VkFramebuffer>     / Connects resources to the render pass.
        - graphics_pipeline          / VkPipeline                / Converts 3D models into 2D frames.
        - graphics_queue             / VkQueue                   / Handles all graphics commands and calls.
        - image_available_semaphores / vector<VkSemaphore>       / Signals that a swap chain image is ready to be used for rendering.
        - index_buffer               / VkBuffer                  / Enables us to reuse vertices to optimize renderings.
        - indices                    / vector<uint32_t>          / Separates all vertices by shapes.
        - logical_device             / VkDevice                  / Logical device of the Vulkan instance.
        - pipeline_layout            / VkPipelineLayout          / Describes binding locations used by the pipeline and descriptor sets.
        - present_queue              / VkQueue                   / Passes a fully rendering frame to display it.
        - render_finished_semaphores / vector<VkSemaphore>       / Signals that the rendering of an image is done.
        - render_pass                / VkRenderPass              / Organizes rendering tasks.
        - scissor                    / VkRect2D                  / Discards any pixels outside of the rendering area.
        - swapchain                  / VkSwapchainKHR            / Queue that simultaneously acquires, renders and presents frames.
        - texture_image_views        / vector<VkImageView>       / Describes how to access and treat the texture images.
        - uniform_buffers            / vector<UniformBufferInfo> / Enables us to transfer some data to shaders.
        - vertex_buffer              / VkBuffer                  / Contails all vertex data.
        - viewport                   / VkViewport                / Defines where the rendering occurs.

    Returns:
        Whether a message on failure/success or a "recreate" message, to warn that the swap chain must be recreated.
*/
std::string Render::draw_frame
(
    const std::vector<VkCommandBuffer>   &command_buffers,
    const std::vector<VkDescriptorSet>   &descriptor_sets,
    const VkExtent2D                     &extent,
    const std::vector<VkFence>           &fences,
    const size_t                         &frame,
    const std::vector<VkFramebuffer>     &framebuffers,
    const VkPipeline                     &graphics_pipeline,
    const VkQueue                        &graphics_queue,
    const std::vector<VkSemaphore>       &image_available_semaphores,
    const VkBuffer                       &index_buffer,
    const std::vector<uint32_t>          &indices,
    const VkDevice                       &logical_device,
    const VkPipelineLayout               &pipeline_layout,
    const VkQueue                        &present_queue,
    const std::vector<VkSemaphore>       &render_finished_semaphores,
    const VkRenderPass                   &render_pass,
    const VkRect2D                       &scissor,
    const VkSwapchainKHR                 &swapchain,
    const std::vector<VkImageView>       &texture_image_views,
    const std::vector<UniformBufferInfo> &uniform_buffers,
    const VkBuffer                       &vertex_buffer,
    const VkViewport                     &viewport
)
{
    if (command_buffers.size() < 1)
        Utils::Logs::crash_log("Failed to draw frame! No command buffers provided.");

    if (descriptor_sets.size() < 1)
        Utils::Logs::crash_log("Failed to draw frame! No descriptor sets provided.");

    if (fences.size() < 1)
        Utils::Logs::crash_log("Failed to draw frame! No fences provided.");

    if (frame >= fences.size())
        Utils::Logs::crash_log("Failed to draw frame! Frame out of fences bounds -> " + std::to_string(frame) + " >= " + std::to_string(fences.size()) + ".");

    if (frame >= image_available_semaphores.size())
        Utils::Logs::crash_log("Failed to draw frame! Frame out of image available semaphores bounds -> " + std::to_string(frame) + " >= " + std::to_string(image_available_semaphores.size()) + ".");

    if (frame >= command_buffers.size())
        Utils::Logs::crash_log("Failed to draw frame! Frame out of command buffers bounds -> " + std::to_string(frame) + " >= " + std::to_string(command_buffers.size()) + ".");

    if (framebuffers.size() < 1)
        Utils::Logs::crash_log("Failed to draw frame! No frame buffers provided.");

    if (graphics_pipeline == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed to draw frame! Graphics pipeline invalid.");

    if (graphics_queue == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed to draw frame! Graphics queue invalid.");

    if (image_available_semaphores.size() < 1)
        Utils::Logs::crash_log("Failed to draw frame! No image available semaphores provided.");

    if (index_buffer == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed to draw frame! Index buffer invalid.");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed to draw frame! Logical device invalid.");

    if (pipeline_layout == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed to draw frame! Pipeline layout invalid.");

    if (present_queue == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed to draw frame! Present queue invalid.");

    if (render_finished_semaphores.size() < 1)
        Utils::Logs::crash_log("Failed to draw frame! No render finished semaphores provided.");

    if (render_pass == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed to draw frame! Render pass invalid.");

    if (swapchain == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed to draw frame! Swap chain invalid.");

    if (texture_image_views.size() < 1)
        Utils::Logs::crash_log("Failed to draw frame! No texture image views provided.");

    if (vertex_buffer == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed to draw frame! Vertex buffer invalid.");

    if (uniform_buffers.size() < 1)
        Utils::Logs::crash_log("Failed to draw frame! No uniform buffers provided.");

    vkWaitForFences(logical_device, 1, &fences[frame], VK_TRUE, UINT64_MAX);

    uint32_t image_index;
    const VkResult acquire_image = vkAcquireNextImageKHR(logical_device, swapchain, UINT64_MAX, image_available_semaphores[frame], VK_NULL_HANDLE, &image_index);

    if (acquire_image == VK_ERROR_OUT_OF_DATE_KHR)
    {
        Utils::Logs::log("Failed to draw frame! Outdated swap chain.", true);
        return "recreate";
    }

    if (acquire_image == VK_SUBOPTIMAL_KHR)
    {
        Utils::Logs::log("Failed to draw frame! Suboptimal swap chain.", true);
        return "recreate";
    }

    if (acquire_image != VK_SUCCESS)
    {
        Utils::Logs::log("Failed to draw frame! Next image acquirement returned error code -> " + std::to_string(acquire_image) + ".", true);
        return "failed";
    }

    if (image_index >= render_finished_semaphores.size())
    {
        Utils::Logs::log("Failed to draw frame! Image index out of semaphores bounds -> " + std::to_string(image_index) + " >= " + std::to_string(render_finished_semaphores.size()) + ".", true);
        return "failed";
    }

    vkResetFences(logical_device, 1, &fences[frame]);
    vkResetCommandBuffer(command_buffers[frame], 0);

    Buffers::record_command_buffer_and_draw(command_buffers[frame], descriptor_sets, extent, frame, framebuffers, graphics_pipeline, image_index, index_buffer, pipeline_layout, render_pass, scissor, texture_image_views, vertex_buffer, indices, viewport);
    Buffers::update_uniform_buffer_data(uniform_buffers[frame].data, extent, frame);

    const VkSemaphore wait_semaphores[] = { image_available_semaphores[frame] };
    const VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    const VkSemaphore signal_semaphores[] = { render_finished_semaphores[image_index] };

    /*
        - sType                / Defines the type of the structure.
        - waitSemaphoreCount   / Amount of semaphores to use to wait.
        - pWaitSemaphores      / Passed the wait semaphores.
        - pWaitDstStageMask    / Passes pipeline stages that wait semaphores will occur to.
        - commandBufferCount   / Amount of command buffers to use.
        - pCommandBuffers      / Passes the command buffers.
        - signalSemaphoreCount / Amount of semaphores to use for signaling.
        - pSignalSemaphores    / Passes the signal semaphores.
    */
    const VkSubmitInfo submit_info
    {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = wait_semaphores,
        .pWaitDstStageMask = wait_stages,
        .commandBufferCount = 1,
        .pCommandBuffers = &command_buffers[frame],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = signal_semaphores
    };

    const VkResult queue_submit = vkQueueSubmit(graphics_queue, 1, &submit_info, fences[frame]);

    if (queue_submit != VK_SUCCESS)
    {
        Utils::Logs::log("Failed to draw frame! Frame submit returned error code -> " + std::to_string(queue_submit) + ".", true);
        return "failed";
    }

    /*
        - sType              / Defines the type of the structure.
        - waitSemaphoreCount / Amount of semaphores to use to wait.
        - pWaitSemaphores    / Passed the wait semaphores.
        - swapchainCount     / Amount of swap chains to pass.
        - pSwapchains        / Passes swap chains.
        - pImageIndices      / Passes indices.
    */
    const VkPresentInfoKHR present_info
    {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = signal_semaphores,
        .swapchainCount = 1,
        .pSwapchains = &swapchain,
        .pImageIndices = &image_index
    };

    const VkResult present_result = vkQueuePresentKHR(present_queue, &present_info);

    if (present_result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        Utils::Logs::log("Failed to draw frame! Outdated swap chain.", true);
        return "recreate";
    }

    if (present_result != VK_SUCCESS)
    {
        Utils::Logs::log("Failed to draw frame! Frame presentation returned error code -> " + std::to_string(present_result) + ".", true);
        return "failed";
    }

    return "success";
}
