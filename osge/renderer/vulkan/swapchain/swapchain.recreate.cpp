#include "vulkan.swapchain.hpp"

#include "../vulkan.renderer.hpp"
#include "../../../utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"
#include "libraries/sdl/SDL3/SDL.h"

#include <cstdint>
#include <string>
#include <vector>

/*
    Recreate a swap chain once the window is not minimized.

    Tasks:
        1) Verify function parameters.
        2) Wait for the window to be either unminimized or closed to continue.
        3) Wait for logical device to idle.
        4) Destroy all objects depending on the swap chain.
        5) Destroy swap chain.
        6) Empty semaphores vector list.
        7) Get swap chain capabilities and select resolution.
        8) Create new swap chain.
        9) Recreate all rendering objects destroyed previously.
        10) Separate all semaphores into two groups: image available and render finished.

    Parameters:
        - color_resources            / color_resources_handler       / Color image view, color image and color image memory.
        - command_pool               / VkCommandPool                 / Handles memory allocation of command buffers.
        - depth_resources            / depth_resources_handler       / Depth image view, depth image and depth image memory.
        - extent                     / VkExtent2D                    / Resolution of the swap chain.
        - framebuffers               / frame_buffers_handler         / Connects resources to the render pass.
        - graphics_family_index      / uint32_t                      / Index of the graphics queue family.
        - graphics_queue             / VkQueue                       / Handles all graphics commands and calls.
        - image_available_semaphores / vector<VkSemaphore>           / Signals that a swap chain image is ready to be used for rendering.
        - logical_device             / VkDevice                      / Logical device of the Vulkan instance.
        - physical_device            / VkPhysicalDevice              / Physical device used to run this Vulkan instance.
        - present_mode               / VkPresentModeKHR              / Defines how rendered images are presented.
        - present_family_index       / uint32_t                      / Index of the present queue family.
        - render_finished_semaphores / vector<VkSemaphore>           / Signals that the rendering of an image is done.
        - render_pass                / VkRenderPass                  / Organizes rendering tasks.
        - samples_count              / VkSampleCountFlagBits         / Amount of samples to use for multisampling.
        - semaphores                 / sync_semaphores_handler       / All image available and render finished semaphores.
        - surface_format             / VkSurfaceFormatKHR            / Defines the surface format to use for the swap chain.
        - swapchain                  / swapchain_handler             / Queue that simultaneously acquires, renders and presents frames.
        - swapchain_image_views      / swapchain_image_views_handler / Describes how to access and treat swap chain images.
        - vulkan_surface             / VkSurfaceKHR                  / Link between this Vulkan instance and the SDL3 game window.
        - window                     / SDL_Window*                   / Window of the game.

    Returns:
        A string being either "success" or "exit" (= user requested to close the app).
*/
std::string Swapchain::recreate_swapchain
(
    Vulkan::Colors::color_resources_handler  &color_resources,
    const VkCommandPool                      &command_pool,
    Vulkan::Depth::depth_resources_handler   &depth_resources,
    VkExtent2D                               &extent,
    Vulkan::Buffers::frame_buffers_handler   &framebuffers,
    const uint32_t                           &graphics_family_index,
    const VkQueue                            &graphics_queue,
    std::vector<VkSemaphore>                 &image_available_semaphores,
    const VkDevice                           &logical_device,
    const VkPhysicalDevice                   &physical_device,
    const VkPresentModeKHR                   &present_mode,
    const uint32_t                           &present_family_index,
    std::vector<VkSemaphore>                 &render_finished_semaphores,
    const VkRenderPass                       &render_pass,
    const VkSampleCountFlagBits              &samples_count,
    Vulkan::Render::sync_semaphores_handler  &semaphores,
    const VkSurfaceFormatKHR                 &surface_format,
    Swapchain::swapchain_handler             &swapchain,
    Swapchain::swapchain_image_views_handler &swapchain_image_views,
    const VkSurfaceKHR                       &vulkan_surface,
    SDL_Window                               *window
)
{
    Utils::Logs::log("Recreating swap chain..", true);

    if (command_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Swap chain recreation failed! Command pool invalid.");

    if (graphics_family_index < 0)
        Utils::Logs::crash_log("Swap chain recreation failed! Graphics family index invalid -> " + std::to_string(graphics_family_index) + ".");

    if (graphics_queue == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Swap chain recreation failed! Graphics queue invalid.");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Swap chain recreation failed! Logical device invalid.");

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Swap chain recreation failed! Physical device invalid.");

    if (present_family_index < 0)
        Utils::Logs::crash_log("Swap chain recreation failed! Present family index invalid -> " + std::to_string(present_family_index) + ".");

    if (render_pass == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Swap chain recreation failed! Render pass invalid.");

    if (samples_count < 1)
        Utils::Logs::crash_log("Swap chain recreation failed! Samples count invalid -> " + std::to_string(samples_count) + ".");

    if (vulkan_surface == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Swap chain recreation failed! Vulkan surface invalid.");

    if (!window)
        Utils::Logs::crash_log("Swap chain recreation failed! SDL3 window invalid -> " + Utils::Text::get_memory_address(window) + ".");

    while (true)
    {
        const Uint32 window_flags = SDL_GetWindowFlags(window);
        const bool minimized = window_flags & SDL_WINDOW_MINIMIZED;

        if (!minimized)
            break;
        else Utils::Logs::log("Waiting for the user to unminimize or close the window to continue..", true);

        SDL_Event event;
        SDL_WaitEvent(&event);

        if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            return "exit";
    }

    vkDeviceWaitIdle(logical_device);

    framebuffers.~frame_buffers_handler();
    swapchain_image_views.~swapchain_image_views_handler();
    semaphores.~sync_semaphores_handler();
    depth_resources.~depth_resources_handler();
    swapchain.~swapchain_handler();

    image_available_semaphores.clear();
    render_finished_semaphores.clear();

    const VkSurfaceCapabilitiesKHR capabilities = Swapchain::get_swapchain_capabilities(physical_device, vulkan_surface);
    extent = Swapchain::select_swapchain_extent(capabilities, window);
    uint32_t images_count = capabilities.minImageCount + 1;

    if (capabilities.maxImageCount > 0 && images_count > capabilities.maxImageCount)
    {
        images_count = capabilities.maxImageCount;
        Utils::Logs::log("Fixed images count that was higher than swap chain capabilities: " + std::to_string(images_count) + " > " + std::to_string(capabilities.maxImageCount) + ".", true);
    }

    new (&swapchain) Swapchain::swapchain_handler (capabilities, extent, graphics_family_index, images_count, logical_device, present_family_index, present_mode, surface_format, vulkan_surface);

    const std::vector<VkImage> new_images = Swapchain::get_swapchain_images(logical_device, swapchain.get());
    new (&swapchain_image_views) Swapchain::swapchain_image_views_handler(surface_format.format, logical_device, new_images);
    new (&depth_resources) Vulkan::Depth::depth_resources_handler(command_pool, extent, graphics_queue, logical_device, physical_device, samples_count);
    new (&semaphores) Vulkan::Render::sync_semaphores_handler(images_count * 2, logical_device);
    new (&color_resources) Vulkan::Colors::color_resources_handler(logical_device, physical_device, samples_count, extent, surface_format.format);
    new (&framebuffers) Vulkan::Buffers::frame_buffers_handler(color_resources.get().color_image_view, depth_resources.get().image_view, extent, swapchain_image_views.get(), logical_device, render_pass);

    int i = 0;

    for (const VkSemaphore &semaphore : semaphores.get())
    {
        i++;

        if (semaphores.get().size() / 2 < i)
        {
            image_available_semaphores.emplace_back(semaphore);
            continue;
        }

        render_finished_semaphores.emplace_back(semaphore);
    }

    Utils::Logs::log("Swap chain recreated.", true);
    return "success";
}
