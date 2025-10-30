#include "swapchain.handler.hpp"
#include "swapchain.images.hpp"
#include "../depth/depth.resources.hpp"
#include "../render/render.framebuffers.hpp"
#include "../render/sync/render.sync.semaphores.hpp"

#include <vulkan/vulkan.h>
#include <cstdint>
#include <SDL3/SDL.h>
#include <vector>
#include <string>

#ifndef VULKAN_SWAPCHAIN_RECREATION_HPP
#define VULKAN_SWAPCHAIN_RECREATION_HPP

std::string recreate_vulkan_swapchain
(
    const VkDevice &logical_device,
    const VkSurfaceKHR &vulkan_surface,
    const VkPhysicalDevice &physical_device,
    const VkSurfaceFormatKHR &surface_format,
    const VkPresentModeKHR &present_mode,
    const uint32_t &graphics_family_index,
    const uint32_t &present_family_index,
    const VkRenderPass &render_pass,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    SDL_Window* window,
    Vulkan_Swapchain &swapchain,
    Vulkan_SwapchainImageViews &image_views,
    Vulkan_Framebuffers &framebuffers,
    Vulkan_DepthResources &depth_resources,
    VkExtent2D &extent,
    Vulkan_Semaphores &semaphores,
    std::vector<VkSemaphore> &image_available_semaphores,
    std::vector<VkSemaphore> &render_finished_semaphores
);

#endif
