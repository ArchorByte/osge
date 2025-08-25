#include <vulkan/vulkan.h>
#include <stdint.h>
#include <vector>

#ifndef VULKAN_COMMAND_BUFFER_RECORDER_HPP
#define VULKAN_COMMAND_BUFFER_RECORDER_HPP

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
    const std::vector<VkImageView> texture_image_views
);

#endif
