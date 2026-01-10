#ifndef VULKAN_BUFFERS_HPP
#define VULKAN_BUFFERS_HPP

#include <libraries/glm/glm.hpp>
#include <libraries/vulkan/vulkan.h>

#include <utility>
#include <vector>

#include "../textures/texture.images.loader.hpp"
#include "../vertex/vertex.handler.hpp"

////////////////////////////////////////////////////
//////////////////// Structures ////////////////////
////////////////////////////////////////////////////

struct Buffer
{
    VkBuffer buffer;
    VkDeviceMemory buffer_memory;
};

struct UniformBufferInfo
{
    VkBuffer buffer;
    VkDeviceMemory buffer_memory;
    void* data;
};

struct UniformBufferObject
{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 projection;
};

///////////////////////////////////////////////////
//////////////////// Namespace ////////////////////
///////////////////////////////////////////////////

namespace Vulkan::Buffers
{
    ///////////////////////////////////////////////////
    //////////////////// Functions ////////////////////
    ///////////////////////////////////////////////////

    ///////////////////////////////
    ///// buffers.handler.cpp /////
    ///////////////////////////////

    void create_buffer
    (
        VkBuffer &buffer,
        VkDeviceMemory &buffer_memory,
        const VkDeviceSize &buffer_size,
        const VkDevice &logical_device,
        const VkPhysicalDevice &physical_device,
        const VkBufferUsageFlags &usage_flags
    );

    void destroy_buffer
    (
        VkBuffer &buffer,
        VkDeviceMemory &buffer_memory,
        const VkDevice &logical_device
    );

    //////////////////////////////
    ///// buffers.memory.cpp /////
    //////////////////////////////

    uint32_t find_memory_type
    (
        const VkPhysicalDeviceMemoryProperties &memory_properties,
        const VkMemoryPropertyFlags &property_flags,
        const uint32_t &type_filter
    );

    VkDeviceMemory allocate_buffer_memory
    (
        const VkBuffer &buffer,
        const VkDevice &logical_device,
        const VkPhysicalDevice &physical_device
    );

    ///////////////////////////////////////
    ///// command.buffer.recorder.cpp /////
    ///////////////////////////////////////

    void record_command_buffer_and_draw
    (
        const VkCommandBuffer &command_buffer,
        const std::vector<VkDescriptorSet> descriptor_sets,
        const VkExtent2D &extent,
        const size_t &frame,
        const std::vector<VkFramebuffer> &framebuffers,
        const VkPipeline &graphics_pipeline,
        const uint32_t &image_index,
        const VkBuffer &index_buffer,
        std::vector<uint32_t> indices,
        const VkPipelineLayout &pipeline_layout,
        const VkRenderPass &render_pass,
        const VkRect2D &scissor,
        const std::vector<VkImageView> texture_image_views,
        const VkBuffer &vertex_buffer,
        const VkViewport &viewport
    );

    ///////////////////////////////
    ///// command.buffers.cpp /////
    ///////////////////////////////

    std::vector<VkCommandBuffer> create_command_buffers
    (
        const VkCommandPool &command_pool,
        const uint32_t &images_count,
        const VkDevice &logical_device
    );

    VkCommandBuffer create_one_time_command_buffer
    (
        const VkCommandPool &command_pool,
        const VkDevice &logical_device
    );

    void destroy_command_buffer
    (
        VkCommandBuffer &command_buffer,
        const VkCommandPool &command_pool,
        const VkQueue &graphics_queue,
        const VkDevice &logical_device
    );

    ///////////////////////////
    ///// copy.buffer.cpp /////
    ///////////////////////////

    void copy_buffer_data
    (
        const VkDeviceSize &buffer_size,
        const VkCommandPool &command_pool,
        const VkBuffer &destination_buffer,
        const VkQueue &graphics_queue,
        const VkDevice &logical_device,
        const VkBuffer &source_buffer
    );

    void copy_buffer_to_texture_image
    (
        const VkCommandPool &command_pool,
        const VkQueue &graphics_queue,
        const VkDevice &logical_device,
        const VkBuffer &source_buffer,
        const VkImage &texture_image,
        const TextureImageInfo &texture_image_info
    );

    /////////////////////////////
    ///// frame.buffers.cpp /////
    /////////////////////////////

    std::vector<VkFramebuffer> create_frame_buffers
    (
        const VkImageView &color_image_view,
        const VkImageView &depth_image_view,
        const VkExtent2D &extent,
        const std::vector<VkImageView> &image_views,
        const VkDevice &logical_device,
        const VkRenderPass &render_pass
    );

    void destroy_frame_buffers
    (
        std::vector<VkFramebuffer> &framebuffers,
        const VkDevice &logical_device
    );

    ////////////////////////////
    ///// index.buffer.cpp /////
    ////////////////////////////

    std::pair<VkBuffer, VkDeviceMemory> create_index_buffer
    (
        const VkCommandPool &command_pool,
        const VkQueue &graphics_queue,
        std::vector<uint32_t> &indices,
        const VkDevice &logical_device,
        const VkPhysicalDevice &physical_device,
        std::vector<Vertex> &vertices
    );

    void destroy_index_buffer
    (
        VkDeviceMemory &buffer_memory,
        VkBuffer &index_buffer,
        const VkDevice &logical_device
    );

    /////////////////////////////////////
    ///// texture.image.buffers.cpp /////
    /////////////////////////////////////

    std::vector<Buffer> create_texture_image_buffers
    (
        const VkDevice &logical_device,
        const VkPhysicalDevice &physical_device,
        const std::vector<TextureImageInfo> &texture_image_info
    );

    void destroy_texture_image_buffers
    (
        const VkDevice &logical_device,
        std::vector<Buffer> &texture_image_buffers
    );

    ///////////////////////////////
    ///// uniform.buffers.cpp /////
    ///////////////////////////////

    std::vector<UniformBufferInfo> create_uniform_buffers
    (
        const VkCommandPool &command_pool,
        const VkQueue &graphics_queue,
        const uint32_t &image_count,
        const VkDevice &logical_device,
        const VkPhysicalDevice &physical_device
    );

    void update_uniform_buffer_data
    (
        const void* buffer_data,
        const VkExtent2D extent,
        const uint32_t &frame
    );

    void destroy_uniform_buffers
    (
        const VkDevice &logical_device,
        std::vector<UniformBufferInfo> &uniform_buffers
    );

    /////////////////////////////
    ///// vertex.buffer.cpp /////
    /////////////////////////////

    std::pair<VkBuffer, VkDeviceMemory> create_vertex_buffer
    (
        const VkCommandPool &command_pool,
        const VkQueue &graphics_queue,
        const VkDevice &logical_device,
        const VkPhysicalDevice &physical_device,
        std::vector<Vertex> &vertices
    );

    void destroy_vertex_buffer
    (
        const VkDevice &logical_device,
        VkBuffer &vertex_buffer,
        VkDeviceMemory &vertex_buffer_memory
    );

    /////////////////////////////////////////////////
    //////////////////// Classes ////////////////////
    /////////////////////////////////////////////////

    /////////////////////////////
    ///// frame.buffers.cpp /////
    /////////////////////////////

    class frame_buffers_handler
    {
        public:
            frame_buffers_handler
            (
                const VkImageView &color_image_view,
                const VkImageView &depth_image_view,
                const VkExtent2D &extent,
                const std::vector<VkImageView> &image_views,
                const VkDevice &logical_device,
                const VkRenderPass &render_pass
            );

            ~frame_buffers_handler();
            std::vector<VkFramebuffer> get() const;

            // Prevent data duplication.
            frame_buffers_handler(const frame_buffers_handler&) = delete;
            frame_buffers_handler &operator = (const frame_buffers_handler&) = delete;
        private:
            std::vector<VkFramebuffer> frame_buffers;
            VkDevice logical_device = VK_NULL_HANDLE;
    };

    ////////////////////////////
    ///// index.buffer.cpp /////
    ////////////////////////////

    class index_buffer_handler
    {
        public:
            index_buffer_handler
            (
                const VkCommandPool &command_pool,
                const VkQueue &graphics_queue,
                std::vector<uint32_t> &indices,
                const VkDevice &logical_device,
                const VkPhysicalDevice &physical_device,
                std::vector<Vertex> &vertices
            );

            ~index_buffer_handler();
            VkBuffer get() const;

            // Prevent data duplication.
            index_buffer_handler(const index_buffer_handler&) = delete;
            index_buffer_handler& operator = (const index_buffer_handler&) = delete;
        private:
            VkDeviceMemory buffer_memory = VK_NULL_HANDLE;
            VkBuffer index_buffer = VK_NULL_HANDLE;
            VkDevice logical_device = VK_NULL_HANDLE;
    };

    /////////////////////////////////////
    ///// texture.image.buffers.cpp /////
    /////////////////////////////////////

    class texture_image_buffers_handler
    {
        public:
            texture_image_buffers_handler
            (
                const VkDevice &logical_device,
                const VkPhysicalDevice &physical_device,
                const std::vector<TextureImageInfo> &texture_image_info
            );

            ~texture_image_buffers_handler();
            std::vector<Buffer> get() const;

            // Prevent data duplication.
            texture_image_buffers_handler (const texture_image_buffers_handler&) = delete;
            texture_image_buffers_handler &operator = (const texture_image_buffers_handler&) = delete;
        private:
            VkDevice logical_device = VK_NULL_HANDLE;
            std::vector<Buffer> texture_image_buffers;
    };

    ///////////////////////////////
    ///// uniform.buffers.cpp /////
    ///////////////////////////////

    class uniform_buffers_handler
    {
        public:
            uniform_buffers_handler
            (
                const VkCommandPool &command_pool,
                const VkQueue &graphics_queue,
                const uint32_t &image_count,
                const VkDevice &logical_device,
                const VkPhysicalDevice &physical_device
            );

            ~uniform_buffers_handler();
            std::vector<UniformBufferInfo> get() const;

            // Prevent data duplication.
            uniform_buffers_handler(const uniform_buffers_handler&) = delete;
            uniform_buffers_handler &operator = (const uniform_buffers_handler&) = delete;
        private:
            VkDevice logical_device = VK_NULL_HANDLE;
            std::vector<UniformBufferInfo> uniform_buffers;
    };

    /////////////////////////////
    ///// vertex.buffer.cpp /////
    /////////////////////////////

    class vertex_buffer_handler
    {
        public:
            vertex_buffer_handler
            (
                const VkCommandPool &command_pool,
                const VkQueue &graphics_queue,
                const VkDevice &logical_device,
                const VkPhysicalDevice &physical_device,
                std::vector<Vertex> &vertices
            );

            ~vertex_buffer_handler();
            VkBuffer get() const;

            // Prevent data duplication.
            vertex_buffer_handler(const vertex_buffer_handler&) = delete;
            vertex_buffer_handler &operator = (const vertex_buffer_handler&) = delete;
        private:
            VkDevice logical_device = VK_NULL_HANDLE;
            VkBuffer vertex_buffer = VK_NULL_HANDLE;
            VkDeviceMemory vertex_buffer_memory = VK_NULL_HANDLE;
    };
}

#endif
