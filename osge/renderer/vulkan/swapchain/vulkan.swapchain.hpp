#ifndef VULKAN_SWAPCHAIN_HPP
#define VULKAN_SWAPCHAIN_HPP

#include "libraries/vulkan/vulkan.h"
#include "libraries/sdl/SDL3/SDL.h"
#include "../buffers/vulkan.buffers.hpp"
#include "../colors/vulkan.colors.hpp"
#include "../depth/vulkan.depth.hpp"
#include "../render/vulkan.render.hpp"

#include <string>
#include <vector>

namespace Swapchain
{
    /////////////////////////////////////////////////
    //////////////////// Classes ////////////////////
    /////////////////////////////////////////////////

    /////////////////////////////////
    ///// swapchain.handler.cpp /////
    /////////////////////////////////

    class swapchain_handler
    {
        public:
            swapchain_handler
            (
                const VkSurfaceCapabilitiesKHR &capabilities,
                const VkExtent2D               &extent,
                const uint32_t                 &graphics_family_index,
                const uint32_t                 &images_count,
                const VkDevice                 &logical_device,
                const uint32_t                 &present_family_index,
                const VkPresentModeKHR         &present_mode,
                const VkSurfaceFormatKHR       &surface_format,
                const VkSurfaceKHR             &vulkan_surface
            );

            ~swapchain_handler();
            VkSwapchainKHR get() const;

            // Prevent data duplication.
            swapchain_handler(const swapchain_handler&) = delete;
            swapchain_handler &operator = (const swapchain_handler&) = delete;
        private:
            VkDevice logical_device = VK_NULL_HANDLE;
            VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    };

    ////////////////////////////////
    ///// swapchain.images.cpp /////
    ////////////////////////////////

    class swapchain_image_views_handler
    {
        public:
            swapchain_image_views_handler
            (
                const VkFormat             &image_format,
                const VkDevice             &logical_device,
                const std::vector<VkImage> &swapchain_images
            );

            ~swapchain_image_views_handler();
            std::vector<VkImageView> get() const;

            // Prevent data duplication.
            swapchain_image_views_handler(const swapchain_image_views_handler&) = delete;
            swapchain_image_views_handler &operator = (const swapchain_image_views_handler&) = delete;
        private:
            VkDevice logical_device = VK_NULL_HANDLE;
            std::vector<VkImageView> swapchain_image_views;
    };

    ///////////////////////////////////////////////////
    //////////////////// Functions ////////////////////
    ///////////////////////////////////////////////////

    //////////////////////////////////
    ///// swapchain.data.queries /////
    //////////////////////////////////

    VkSurfaceCapabilitiesKHR get_swapchain_capabilities
    (
        const VkPhysicalDevice &physical_device,
        const VkSurfaceKHR     &vulkan_surface
    );

    std::vector<VkSurfaceFormatKHR> get_swapchain_surface_formats
    (
        const VkPhysicalDevice &physical_device,
        const VkSurfaceKHR     &vulkan_surface
    );

    std::vector<VkPresentModeKHR> get_swapchain_present_modes
    (
        const VkPhysicalDevice &physical_device,
        const VkSurfaceKHR     &vulkan_surface
    );

    ////////////////////////////////////////
    ///// swapchain.data.selection.cpp /////
    ////////////////////////////////////////

    VkSurfaceFormatKHR select_best_swapchain_surface_format
    (
        const std::vector<VkSurfaceFormatKHR> &formats
    );

    VkPresentModeKHR select_best_swapchain_present_mode
    (
        const std::vector<VkPresentModeKHR> &present_modes
    );

    VkExtent2D select_swapchain_extent
    (
        const VkSurfaceCapabilitiesKHR &capabilities,
        SDL_Window                     *window
    );

    /////////////////////////////////
    ///// swapchain.handler.cpp /////
    /////////////////////////////////

    VkSwapchainKHR create_swapchain
    (
        const VkSurfaceCapabilitiesKHR &capabilities,
        const VkExtent2D               &extent,
        const uint32_t                 &graphics_family_index,
        const uint32_t                 &images_count,
        const VkDevice                 &logical_device,
        const uint32_t                 &present_family_index,
        const VkPresentModeKHR         &present_mode,
        const VkSurfaceFormatKHR       &surface_format,
        const VkSurfaceKHR             &vulkan_surface
    );

    void destroy_swapchain
    (
        const VkDevice &logical_device,
        VkSwapchainKHR &swapchain
    );

    ////////////////////////////////
    ///// swapchain.images.cpp /////
    ////////////////////////////////

    std::vector<VkImage> get_swapchain_images
    (
        const VkDevice       &logical_device,
        const VkSwapchainKHR &swapchain
    );

    std::vector<VkImageView> create_swapchain_image_views
    (
        const VkFormat             &image_format,
        const VkDevice             &logical_device,
        const std::vector<VkImage> &swapchain_images
    );

    void destroy_swapchain_image_views
    (
        std::vector<VkImageView> &image_views,
        const VkDevice           &logical_device
    );

    ////////////////////////////////////
    ///// swapchain.recreation.cpp /////
    ////////////////////////////////////

    std::string recreate_swapchain
    (
        Colors::color_resources_handler &color_resources,
        const VkCommandPool                     &command_pool,
        Depth::depth_resources_handler  &depth_resources,
        VkExtent2D                              &extent,
        Buffers::frame_buffers_handler  &framebuffers,
        const uint32_t                          &graphics_family_index,
        const VkQueue                           &graphics_queue,
        std::vector<VkSemaphore>                &image_available_semaphores,
        const VkDevice                          &logical_device,
        const VkPhysicalDevice                  &physical_device,
        const VkPresentModeKHR                  &present_mode,
        const uint32_t                          &present_family_index,
        std::vector<VkSemaphore>                &render_finished_semaphores,
        const VkRenderPass                      &render_pass,
        const VkSampleCountFlagBits             &samples_count,
        Render::sync_semaphores_handler &semaphores,
        const VkSurfaceFormatKHR                &surface_format,
        swapchain_handler                       &swapchain,
        swapchain_image_views_handler           &swapchain_image_views,
        const VkSurfaceKHR                      &vulkan_surface,
        SDL_Window                              *window
    );
}

#endif
