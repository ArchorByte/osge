#ifndef VULKAN_DESCRIPTORS_HPP
#define VULKAN_DESCRIPTORS_HPP

#include "../buffers/vulkan.buffers.hpp"

#include <vector>
#include <vulkan/vulkan.h>

namespace Vulkan::Descriptors
{
    ///////////////////////////////////////////////////
    //////////////////// Functions ////////////////////
    ///////////////////////////////////////////////////

    ///////////////////////////////
    ///// descriptor.pool.cpp /////
    ///////////////////////////////

    VkDescriptorPool create_descriptor_pool
    (
        const uint32_t &images_count,
        const VkDevice &logical_device,
        const uint32_t &texture_images_count
    );

    void destroy_descriptor_pool
    (
        VkDescriptorPool &descriptor_pool,
        const VkDevice &logical_device
    );

    /////////////////////////////////////
    ///// descriptor.set.layout.cpp /////
    /////////////////////////////////////

    VkDescriptorSetLayout create_descriptor_set_layout
    (
        const VkDevice &logical_device,
        const std::vector<VkImageView> &texture_image_views
    );

    void destroy_descriptor_set_layout
    (
        VkDescriptorSetLayout &descriptor_set_layout,
        const VkDevice &logical_device
    );

    ///////////////////////////////
    ///// descriptor.sets.cpp /////
    ///////////////////////////////

    std::vector<VkDescriptorSet> create_descriptor_sets
    (
        const VkDescriptorSetLayout &descriptor_set_layout,
        const VkDescriptorPool &descriptor_pool,
        const uint32_t &image_count,
        const VkDevice &logical_device,
        const std::vector<VkImageView> &texture_image_views,
        const VkSampler &texture_sampler,
        const std::vector<UniformBufferInfo> &uniform_buffers
    );

    void destroy_descriptor_sets
    (
        const VkDescriptorPool &descriptor_pool,
        std::vector<VkDescriptorSet> &descriptor_sets,
        const VkDevice &logical_device
    );

    /////////////////////////////////////////////////
    //////////////////// Classes ////////////////////
    /////////////////////////////////////////////////

    ///////////////////////////////
    ///// descriptor.pool.cpp /////
    ///////////////////////////////

    class descriptor_pool_handler
    {
        public:
            descriptor_pool_handler
            (
                const uint32_t &image_count,
                const VkDevice &logical_device,
                const uint32_t &texture_image_count
            );

            ~descriptor_pool_handler();
            VkDescriptorPool get() const;

            // Prevent data duplication.
            descriptor_pool_handler(const descriptor_pool_handler&) = delete;
            descriptor_pool_handler &operator = (const descriptor_pool_handler&) = delete;
        private:
            VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
            VkDevice logical_device = VK_NULL_HANDLE;
    };

    /////////////////////////////////////
    ///// descriptor.set.layout.cpp /////
    /////////////////////////////////////

    class descriptor_set_layout_handler
    {
        public:
            descriptor_set_layout_handler
            (
                const VkDevice &logical_device,
                const std::vector<VkImageView> &texture_image_views
            );

            ~descriptor_set_layout_handler();
            VkDescriptorSetLayout get() const;

            // Prevent data duplication.
            descriptor_set_layout_handler(const descriptor_set_layout_handler&) = delete;
            descriptor_set_layout_handler &operator = (const descriptor_set_layout_handler&) = delete;
        private:
            VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
            VkDevice logical_device = VK_NULL_HANDLE;
    };

    ///////////////////////////////
    ///// descriptor.sets.cpp /////
    ///////////////////////////////

    class descriptor_sets_handler
    {
        public:
            descriptor_sets_handler
            (
                const VkDescriptorSetLayout &descriptor_set_layout,
                const VkDescriptorPool &descriptor_pool,
                const uint32_t &image_count,
                const VkDevice &logical_device,
                const std::vector<VkImageView> &texture_image_views,
                const VkSampler &texture_sampler,
                const std::vector<UniformBufferInfo> &uniform_buffers
            );

            ~descriptor_sets_handler();
            std::vector<VkDescriptorSet> get() const;

            // Prevent data duplication.
            descriptor_sets_handler(const descriptor_sets_handler&) = delete;
            descriptor_sets_handler &operator = (const descriptor_sets_handler&) = delete;
        private:
            VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
            std::vector<VkDescriptorSet> descriptor_sets;
            VkDevice logical_device = VK_NULL_HANDLE;
    };
}

#endif
