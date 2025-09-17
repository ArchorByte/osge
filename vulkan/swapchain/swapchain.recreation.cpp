#include "swapchain.recreation.hpp"

#include "swapchain.data.queries.hpp"
#include "swapchain.data.selection.hpp"
#include "swapchain.handler.hpp"
#include "swapchain.images.hpp"
#include "../render/render.framebuffers.hpp"
#include "../render/sync/render.sync.semaphores.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <cstdint>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <string>

// Recreate a swap chain.
// Note: If the user requested to stop the app, we return an "exit" message.
std::string recreate_vulkan_swapchain
(
    Vulkan_Swapchain &swapchain,
    Vulkan_SwapchainImageViews &image_views,
    Vulkan_Framebuffers &framebuffers,
    const VkDevice &logical_device,
    const VkSurfaceKHR &vulkan_surface,
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
)
{
    log("Recreating a swap chain..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain recreation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (!vulkan_surface || vulkan_surface == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain recreation failed! The Vulkan surface provided (" + force_string(vulkan_surface) + ") is not valid!");
    }

    if (!physical_device || physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain recreation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (!render_pass || render_pass == VK_NULL_HANDLE)
    {
        fatal_error_log("Swap chain recreation failed! The render pass provided (" + force_string(render_pass) + ") is not valid!");
    }

    if (!window)
    {
        fatal_error_log("Swap chain recreation failed! The window provided (" + force_string(window) + ") is not valid!");
    }

    if (graphics_family_index < 0)
    {
        fatal_error_log("Swap chain recreation failed! The graphics family index provided (" + std::to_string(graphics_family_index) + ") is not valid!");
    }

    if (present_family_index < 0)
    {
        fatal_error_log("Swap chain recreation failed! The present family index provided (" + std::to_string(present_family_index) + ") is not valid!");
    }

    // Check if the window is still minimized.
    while (true)
    {
        Uint32 window_flags = SDL_GetWindowFlags(window);
        bool minimized = window_flags & SDL_WINDOW_MINIMIZED;

        if (!minimized)
        {
            break;
        }
        else log("Waiting for the user to unminimize or close the window to continue..");

        SDL_Event event;
        SDL_WaitEvent(&event);  // This line blocks the code until something happens to the window.

        if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
        {
            return "exit";
        }
    }

    // Wait for the logical device to be idle.
    vkDeviceWaitIdle(logical_device);

    // Destroy old objects.
    framebuffers.~Vulkan_Framebuffers();
    image_views.~Vulkan_SwapchainImageViews();
    swapchain.~Vulkan_Swapchain();
    semaphores.~Vulkan_Semaphores();

    // Empty the semaphores lists.
    image_available_semaphores.clear();
    render_finished_semaphores.clear();

    // Update the info.
    VkSurfaceCapabilitiesKHR swapchain_capabilities = get_vulkan_swapchain_capabilities(physical_device, vulkan_surface); // Query swap chain capabilities.
    extent = select_vulkan_swapchain_extent_resolution(swapchain_capabilities, window);                                   // Select the new extent resolution.
    uint32_t images_count = swapchain_capabilities.minImageCount + 1;                                                     // Get a new images count.

    // Our images count is higher than the swapchain capabilities.
    if (swapchain_capabilities.maxImageCount > 0 && images_count > swapchain_capabilities.maxImageCount)
    {
        images_count = swapchain_capabilities.maxImageCount;
        error_log("Fixed the images count which was higher than the swapchain capabilities: " + std::to_string(images_count) + " > " + std::to_string(swapchain_capabilities.maxImageCount) + ".");
    }

    // Create a new swapchain.
    new (&swapchain) Vulkan_Swapchain
    (
        logical_device,
        swapchain_capabilities,
        present_mode,
        vulkan_surface,
        surface_format,
        extent,
        graphics_family_index,
        present_family_index,
        images_count
    );

    std::vector<VkImage> new_images = get_vulkan_swapchain_images(logical_device, swapchain.get());  // Query the swap chain images.
    new (&image_views) Vulkan_SwapchainImageViews(logical_device, new_images, surface_format.format);        // Create new images views.
    new (&framebuffers) Vulkan_Framebuffers(logical_device, image_views.get(), extent, render_pass); // Create new framebuffers.
    new (&semaphores) Vulkan_Semaphores(logical_device, images_count * 2);                           // Create new semaphores.

    int i = 0;

    // Separate the semaphores into two lists.
    for (const VkSemaphore &semaphore : semaphores.get())
    {
        i++;

        // If we passed the middle of the original list, we add the semaphores in the image available list.
        if (semaphores.get().size() / 2 < i)
        {
            image_available_semaphores.emplace_back(semaphore);
            continue;
        }

        render_finished_semaphores.emplace_back(semaphore);
    }

    log("The swap chain recreation has ended successfully!");
    return "success";
}
