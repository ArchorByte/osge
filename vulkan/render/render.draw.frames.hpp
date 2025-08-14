#include "../uniform/buffers.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

#ifndef VULKAN_RENDER_DRAW_HPP
#define VULKAN_RENDER_DRAW_HPP

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
    const size_t &current_frame,
    const VkBuffer &vertex_buffer,
    const VkBuffer &index_buffer,
    const std::vector<UniformBufferInfo> &uniform_buffers,
    const VkPipelineLayout &pipeline_layout,
    const std::vector<VkDescriptorSet> descriptor_sets
);

#endif
