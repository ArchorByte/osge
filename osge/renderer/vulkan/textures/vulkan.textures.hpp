#ifndef VULKAN_TEXTURES_HPP
#define VULKAN_TEXTURES_HPP

#include "../buffers/vulkan.buffers.hpp"
#include "../../../../libraries/stb/stb_image.h"
#include "../../../../libraries/vulkan/vulkan.h"

#include <string>
#include <vector>

////////////////////////////////////////////////////
//////////////////// Structures ////////////////////
////////////////////////////////////////////////////

struct TextureImageData
{
    std::string    name;
    VkImage        texture_image;
    VkDeviceMemory image_memory;
    uint32_t       mip_levels;
};

struct TextureImageInfo
{
    std::string  name;
    int          channels;
    int          height;
    uint32_t     mip_levels;
    stbi_uc*     pixels;
    VkDeviceSize size;
    int          width;
};

///////////////////////////////////////////////////
//////////////////// Namespace ////////////////////
///////////////////////////////////////////////////

namespace Textures
{
    ///////////////////////////////////////////////////
    //////////////////// Functions ////////////////////
    ///////////////////////////////////////////////////

    ///////////////////////////////
    ///// mipmaps.handler.cpp /////
    ///////////////////////////////

    void generate_mipmaps
    (
        const VkCommandPool    &command_pool,
        const VkFormat         &format,
        const VkQueue          &graphics_queue,
        const int              &height,
        const VkImage          &image,
        const VkDevice         &logical_device,
        const uint32_t         &mip_levels,
        const VkPhysicalDevice &physical_device,
        const int              &width
    );

    ///////////////////////////////////
    ///// texture.image.views.cpp /////
    ///////////////////////////////////

    std::vector<VkImageView> create_texture_image_views
    (
        const VkDevice                  &logical_device,
        const std::vector<TextureImageData> &texture_images
    );

    std::vector<TextureImageInfo> load_textures();

    void destroy_texture_image_views
    (
        const VkDevice           &logical_device,
        std::vector<VkImageView> &texture_image_views
    );

    void free_textures
    (
        std::vector<TextureImageInfo> &all_texture_information
    );

    //////////////////////////////////////
    ///// texture.images.handler.cpp /////
    //////////////////////////////////////

    std::vector<TextureImageData> create_texture_images
    (
        const VkCommandPool                 &command_pool,
        const VkQueue                       &graphics_queue,
        const VkDevice                      &logical_device,
        const VkPhysicalDevice              &physical_device,
        const std::vector<Buffer>           &texture_image_buffers,
        const std::vector<TextureImageInfo> &texture_image_info
    );

    void destroy_texture_images
    (
        const VkDevice                &logical_device,
        std::vector<TextureImageData> &texture_images
    );

    ///////////////////////////////
    ///// texture.sampler.cpp /////
    ///////////////////////////////

    VkSampler create_texture_sampler
    (
        const VkDevice         &logical_device,
        const VkPhysicalDevice &physical_device
    );

    void destroy_texture_sampler
    (
        const VkDevice &logical_device,
        VkSampler      &texture_sampler
    );

    /////////////////////////////////////////////////
    //////////////////// Classes ////////////////////
    /////////////////////////////////////////////////

    ///////////////////////////////////
    ///// texture.image.views.cpp /////
    ///////////////////////////////////

    class texture_image_views_handler
    {
        public:
            texture_image_views_handler
            (
                const VkDevice                      &logical_device,
                const std::vector<TextureImageData> &texture_images
            );

            ~texture_image_views_handler();
            std::vector<VkImageView> get() const;

            // Prevent data duplication.
            texture_image_views_handler(const texture_image_views_handler&) = delete;
            texture_image_views_handler &operator = (const texture_image_views_handler&) = delete;
        private:
            std::vector<VkImageView> texture_image_views;
            VkDevice logical_device = VK_NULL_HANDLE;
    };

    //////////////////////////////////////
    ///// texture.images.handler.cpp /////
    //////////////////////////////////////

    class texture_images_handler
    {
        public:
            texture_images_handler
            (
                const VkCommandPool                 &command_pool,
                const VkQueue                       &graphics_queue,
                const VkDevice                      &logical_device,
                const VkPhysicalDevice              &physical_device,
                const std::vector<Buffer>           &texture_image_buffers,
                const std::vector<TextureImageInfo> &texture_image_info
            );

            ~texture_images_handler();
            std::vector<TextureImageData> get() const;

            // Prevent data duplication.
            texture_images_handler (const texture_images_handler&) = delete;
            texture_images_handler &operator = (const texture_images_handler&) = delete;
        private:
            VkDevice logical_device = VK_NULL_HANDLE;
            std::vector<TextureImageData> texture_images;
    };

    class loaded_textures_handler
    {
        public:
            loaded_textures_handler();
            ~loaded_textures_handler();
            std::vector<TextureImageInfo> get() const;

            // Prevent data duplication.
            loaded_textures_handler (const loaded_textures_handler&) = delete;
            loaded_textures_handler &operator = (const loaded_textures_handler&) = delete;
        private:
            std::vector<TextureImageInfo> texture_image_information;
    };

    ///////////////////////////////
    ///// texture.sampler.cpp /////
    ///////////////////////////////

    class texture_sampler_handler
    {
        public:
            texture_sampler_handler
            (
                const VkDevice         &logical_device,
                const VkPhysicalDevice &physical_device
            );

            ~texture_sampler_handler();
            VkSampler get() const;

            // Prevent data duplication.
            texture_sampler_handler(const texture_sampler_handler&) = delete;
            texture_sampler_handler &operator = (const texture_sampler_handler&) = delete;
        private:
            VkSampler texture_sampler = VK_NULL_HANDLE;
            VkDevice logical_device = VK_NULL_HANDLE;
    };
}

#endif
