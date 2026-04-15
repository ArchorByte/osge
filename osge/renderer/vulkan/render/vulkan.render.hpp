#ifndef VULKAN_RENDER_HPP
#define VULKAN_RENDER_HPP

#include "../buffers/vulkan.buffers.hpp"
#include "libraries/vulkan/vulkan.h"

#include <string>
#include <vector>

namespace Render
{
    ///////////////////////////////////////////////////
    //////////////////// Functions ////////////////////
    ///////////////////////////////////////////////////

    ///////////////////////////
    ///// draw.frames.cpp /////
    ///////////////////////////

    std::string draw_frame
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
    );

    /////////////////////////////
    ///// multisampling.cpp /////
    /////////////////////////////

    VkSampleCountFlagBits get_max_sample_count
    (
        const VkSampleCountFlagBits &custom_count,
        const VkPhysicalDevice      &physical_device
    );

    VkSampleCountFlagBits to_sample_count_flags_bit
    (
        const int &sample_count
    );

    ///////////////////////////
    ///// render.pass.cpp /////
    ///////////////////////////

    VkRenderPass create_render_pass
    (
        const VkAttachmentDescription &color_attachment,
        const VkAttachmentDescription &depth_attachment,
        const VkDevice                &logical_device,
        const VkSurfaceFormatKHR      &surface_format
    );

    void destroy_render_pass
    (
        const VkDevice &logical_device,
        VkRenderPass   &render_pass
    );

    ///////////////////////////
    ///// sync.fences.cpp /////
    ///////////////////////////

    std::vector<VkFence> create_fences
    (
        const uint32_t &images_count,
        const VkDevice &logical_device
    );

    ///////////////////////////////
    ///// sync.semaphores.cpp /////
    ///////////////////////////////

    std::vector<VkSemaphore> create_semaphores
    (
        const uint32_t &images_count,
        const VkDevice &logical_device
    );

    void destroy_semaphores
    (
        const VkDevice           &logical_device,
        std::vector<VkSemaphore> &semaphores
    );

    /////////////////////////////////////////////////
    //////////////////// Classes ////////////////////
    /////////////////////////////////////////////////

    ///////////////////////////
    ///// render.pass.cpp /////
    ///////////////////////////

    class render_pass_handler
    {
        public:
            render_pass_handler
            (
                const VkAttachmentDescription &color_attachment,
                const VkAttachmentDescription &depth_attachment,
                const VkDevice                &logical_device,
                const VkSurfaceFormatKHR      &surface_format
            );

            ~render_pass_handler();
            VkRenderPass get() const;

            // Prevent data duplication.
            render_pass_handler(const render_pass_handler&) = delete;
            render_pass_handler &operator = (const render_pass_handler&) = delete;
        private:
            VkDevice logical_device = VK_NULL_HANDLE;
            VkRenderPass render_pass = VK_NULL_HANDLE;
    };

    ///////////////////////////
    ///// sync.fences.cpp /////
    ///////////////////////////

    class sync_fences_handler
    {
        public:
            sync_fences_handler
            (
                const uint32_t &images_count,
                const VkDevice &logical_device
            );

            ~sync_fences_handler();
            std::vector<VkFence> get() const;

            // Prevent data duplication.
            sync_fences_handler(const sync_fences_handler&) = delete;
            sync_fences_handler &operator = (const sync_fences_handler&) = delete;
        private:
            std::vector<VkFence> fences;
            VkDevice logical_device = VK_NULL_HANDLE;
    };

    ///////////////////////////////
    ///// sync.semaphores.cpp /////
    ///////////////////////////////

    class sync_semaphores_handler
    {
        public:
            sync_semaphores_handler
            (
                const uint32_t &images_count,
                const VkDevice &logical_device
            );

            ~sync_semaphores_handler();
            std::vector<VkSemaphore> get() const;

            // Prevent data duplication.
            sync_semaphores_handler(const sync_semaphores_handler&) = delete;
            sync_semaphores_handler &operator = (const sync_semaphores_handler&) = delete;
        private:
            VkDevice logical_device = VK_NULL_HANDLE;
            std::vector<VkSemaphore> semaphores;
    };
}

#endif
