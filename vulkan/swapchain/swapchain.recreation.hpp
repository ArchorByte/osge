#include "swapchain.handler.hpp"
#include "swapchain.images.hpp"
#include "../render/render.framebuffers.hpp"
#include "../render/sync/render.sync.semaphores.hpp"

#include <vulkan/vulkan.h>
#include <cstdint>
#include <SDL3/SDL.h>
#include <vector>

#ifndef VULKAN_SWAPCHAIN_RECREATION_HPP
#define VULKAN_SWAPCHAIN_RECREATION_HPP

void recreate_vulkan_swapchain
(
    Vulkan_Swapchain &swapchain,
    Vulkan_SwapchainImageViews &image_views,
    Vulkan_Framebuffers &framebuffers,
    const VkDevice &logical_device,
    const VkSurfaceKHR &surface,
    const VkPhysicalDevice &physical_device,
    const VkSurfaceFormatKHR &surface_format,
    const VkPresentModeKHR &present_mode,
    const uint32_t &graphics_family_index,
    const uint32_t &present_family_index,
    const VkRenderPass &render_pass,
    VkExtent2D &extent,
    SDL_Window* window,
    Vulkan_Semaphores &semaphores,
    std::vector<VkSemaphore> &image_available_semaphores,
    std::vector<VkSemaphore> &render_finished_semaphores
);

#endif
