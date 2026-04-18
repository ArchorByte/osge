#include "vulkan.renderer.hpp"

#include "../../../config/engine.config.hpp"
#include "../../utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"
#include "libraries/sdl/SDL3/SDL.h"
#include "../../../new_project/project.hpp"

#include <map>
#include <string>
#include <unistd.h>
#include <vector>

/*
    Run the game with the Vulkan graphics API.

    Tasks:
        1) Verify layers support.
        2) Initialize a Vulkan instance.
        3) Create a Vulkan surface to link this instance to the game window for rendering.
        4) Select a device that will handle rendering.
        5) Select an amount of samples to use for multisampling.
        6) Verify extensions support.
        7) Get swap chain capabilities.
        8) Select a swap chain resolution depending on its capabilities and the game window resolution.
        9) Get surface formats and select the best one.
        10) Get present modes and select the best one.
        11) Select the swap chain images count.
        12) Get the graphics and present family indices.
        13) Create a logical device.
        14) Get graphics and present queues.
        15) Create a swap chain.
        16) Get swap chain images and create image views for them.
        17) Load shaders and create modules and stages for them.
        18) Create a viewport and scissor for the swap chain.
        19) Load all 3D model vertices.
        20) Create a command pool.
        21) Create command buffers.
        22) Create a vertex and index buffer.
        23) Create uniform buffers.
        24) Create color resources.
        25) Create depth resources.
        26) Create a render pass.
        27) Create frame buffers.
        28) Create fences and semaphores to synch renderings.
        29) Separate all semaphores into two groups: image available and render finished semaphores.
        30) Load all textures and create buffers, images views and a sampler for them.
        31) Create a descriptor layout, a pool and sets.
        32) Create graphics pipeline attachments and components.
        33) Create a pipeline layout for the graphics pipeline.
        34) Create a graphics pipeline.
        35) For each frame of the game:
            - Listen for any window event.
            - Render and draw a frame.
            - Switch frame index.
            - Recreate the swap chain if necessary.
            - Run game's code once.
        36) On exit, wait for all objects to idle.

    Parameters:
        - custom_samples_count / int         / Amount of samples to use for multisampling requested by the user.
        - gpu_index            / int         / Index of the GPU that the user wants to use.
        - window               / SDL_Window* / Window of the game.

    Returns:
        No object returned.
*/
void Vulkan::run_game
(
    const int  &custom_samples_count,
    int        &gpu_index,
    SDL_Window *window
)
{
    std::vector<const char*> layers;

    if constexpr (EngineConfig::DEBUG_MODE)
        layers.push_back("VK_LAYER_KHRONOS_validation");

    Vulkan::Core::check_layers_support(layers);

    const Vulkan::Core::vulkan_instance_handler vulkan_instance(layers);
    const Vulkan::Core::vulkan_surface_handler vulkan_surface (vulkan_instance.get(), window);

    const VkPhysicalDevice physical_device = Vulkan::Devices::select_physical_device(gpu_index, vulkan_instance.get());
    const VkSampleCountFlagBits samples_count = Vulkan::Render::get_max_sample_count(Vulkan::Render::to_sample_count_flags_bit(custom_samples_count), physical_device);

    const std::vector<const char*> extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    Vulkan::Core::check_extensions_support(physical_device, extensions);

    const VkSurfaceCapabilitiesKHR capabilities = Vulkan::Swapchain::get_swapchain_capabilities(physical_device, vulkan_surface.get());
    VkExtent2D extent = Vulkan::Swapchain::select_swapchain_extent(capabilities, window);

    const std::vector<VkSurfaceFormatKHR> surface_formats = Vulkan::Swapchain::get_swapchain_surface_formats(physical_device, vulkan_surface.get());
    const VkSurfaceFormatKHR surface_format = Vulkan::Swapchain::select_best_swapchain_surface_format(surface_formats);

    const std::vector<VkPresentModeKHR> present_modes = Vulkan::Swapchain::get_swapchain_present_modes(physical_device, vulkan_surface.get());
    const VkPresentModeKHR present_mode = Vulkan::Swapchain::select_best_swapchain_present_mode(present_modes);

    uint32_t images_count = capabilities.minImageCount + 1;

    if (capabilities.maxImageCount > 0 && images_count > capabilities.maxImageCount)
    {
        images_count = capabilities.maxImageCount;
        Utils::Logs::log("Fixed images count that was higher than swap chain capabilities: " + std::to_string(images_count) + " > " + std::to_string(capabilities.maxImageCount) + ".", true);
    }

    const std::vector<VkQueueFamilyProperties> queue_families = Vulkan::Queues::get_queue_families(physical_device);
    const uint32_t graphics_family_index = Vulkan::Queues::get_graphics_queue_family_index(queue_families);
    const uint32_t present_family_index = Vulkan::Queues::get_present_queue_family_index(physical_device, queue_families, vulkan_surface.get());

    std::vector<uint32_t> queue_indices = { graphics_family_index, present_family_index };
    const std::vector<VkDeviceQueueCreateInfo> queues_create_info = Vulkan::Queues::make_queues_create_info(physical_device, queue_indices);

    const Vulkan::Devices::logical_device_handler logical_device(physical_device, queues_create_info, extensions);

    VkQueue graphics_queue, present_queue;
    vkGetDeviceQueue(logical_device.get(), graphics_family_index, 0, &graphics_queue);
    vkGetDeviceQueue(logical_device.get(), present_family_index, 0, &present_queue);

    Vulkan::Swapchain::swapchain_handler swapchain(capabilities, extent, graphics_family_index, images_count, logical_device.get(), present_family_index, present_mode, surface_format, vulkan_surface.get());
    const std::vector<VkImage> swapchain_images = Vulkan::Swapchain::get_swapchain_images(logical_device.get(), swapchain.get());
    Vulkan::Swapchain::swapchain_image_views_handler swapchain_images_views(surface_format.format, logical_device.get(), swapchain_images);

    const Vulkan::Shaders::shader_modules_handler shaders_modules(logical_device.get());
    const std::vector<VkPipelineShaderStageCreateInfo> shaders_stages = Vulkan::Shaders::create_shader_stages(shaders_modules.get());

    const VkViewport viewport = Vulkan::Pipeline::create_viewport(extent);
    const VkRect2D scissor = Vulkan::Pipeline::create_scissor(extent);

    std::vector<Vulkan::Vertex::VertexObj> vertices;
    std::vector<uint32_t> indices;
    Vulkan::Models::load_3d_models(indices, vertices);

    const Vulkan::Queues::command_pool_handler command_pool(graphics_family_index, logical_device.get());
    const std::vector<VkCommandBuffer> command_buffers = Vulkan::Buffers::create_command_buffers(command_pool.get(), images_count, logical_device.get());
    const Vulkan::Buffers::vertex_buffer_handler vertex_buffer(command_pool.get(), graphics_queue, logical_device.get(), physical_device, vertices);
    const Vulkan::Buffers::index_buffer_handler index_buffer(command_pool.get(), graphics_queue, logical_device.get(), physical_device, indices, vertices);
    const Vulkan::Buffers::uniform_buffers_handler uniform_buffers(command_pool.get(), graphics_queue, images_count, logical_device.get(), physical_device);

    Vulkan::Colors::color_resources_handler color_resources(logical_device.get(), physical_device, samples_count, extent, surface_format.format);
    const VkAttachmentDescription color_attachment = Vulkan::Colors::create_color_attachment(samples_count, surface_format.format);

    Vulkan::Depth::depth_resources_handler depth_resources(command_pool.get(), extent, graphics_queue, logical_device.get(), physical_device, samples_count);
    const VkAttachmentDescription depth_attachment = Vulkan::Depth::create_depth_attachment(physical_device, samples_count);

    const Vulkan::Render::render_pass_handler render_pass(color_attachment, depth_attachment, logical_device.get(), surface_format);
    Vulkan::Buffers::frame_buffers_handler framebuffers(color_resources.get().color_image_view, depth_resources.get().image_view, extent, swapchain_images_views.get(), logical_device.get(), render_pass.get());
    const Vulkan::Render::sync_fences_handler fences(images_count, logical_device.get());
    Vulkan::Render::sync_semaphores_handler semaphores((images_count * 2), logical_device.get());

    const int fences_count = fences.get().size();
    const int semaphores_count = semaphores.get().size();

    if (fences_count != semaphores_count / 2)
        Utils::Logs::crash_log("Semaphores and fences count mismatch -> " + std::to_string(fences.get().size()) + " != " + std::to_string(semaphores_count / 2) + ".");

    if (semaphores_count % 2 != 0)
        Utils::Logs::crash_log("Amount of semaphores (" + std::to_string(semaphores_count) + ") not even.");

    std::vector<VkSemaphore> image_available_semaphores;
    image_available_semaphores.reserve(semaphores_count / 2);

    std::vector<VkSemaphore> render_finished_semaphores;
    render_finished_semaphores.reserve(semaphores_count / 2);

    int i = 0;

    for (const VkSemaphore &semaphore : semaphores.get())
    {
        i++;

        if (semaphore == VK_NULL_HANDLE)
            Utils::Logs::crash_log("Semaphore #" + std::to_string(i) + "/" + std::to_string(semaphores_count) + " not valid.");

        if (semaphores_count / 2 < i)
            image_available_semaphores.emplace_back(semaphore);
        else render_finished_semaphores.emplace_back(semaphore);
    }

    const Vulkan::Textures::loaded_textures_handler loaded_texture_images;
    Vulkan::Buffers::texture_image_buffers_handler texture_image_buffers(logical_device.get(), physical_device, loaded_texture_images.get());
    const Vulkan::Textures::texture_images_handler texture_images(command_pool.get(), graphics_queue, logical_device.get(), physical_device, texture_image_buffers.get(), loaded_texture_images.get());
    const Vulkan::Textures::texture_image_views_handler texture_image_views(logical_device.get(), texture_images.get());
    const Vulkan::Textures::texture_sampler_handler texture_sampler(logical_device.get(), physical_device);

    const Vulkan::Descriptors::descriptor_set_layout_handler descriptor_set_layout(logical_device.get(), texture_image_views.get());
    const Vulkan::Descriptors::descriptor_pool_handler descriptor_pool(images_count, logical_device.get(), texture_images.get().size());
    const Vulkan::Descriptors::descriptor_sets_handler descriptor_sets(descriptor_set_layout.get(), descriptor_pool.get(), images_count, logical_device.get(), texture_image_views.get(), texture_sampler.get(), uniform_buffers.get());

    const VkPipelineInputAssemblyStateCreateInfo assembly_state = Vulkan::Pipeline::create_assembly_state();
    const VkPipelineColorBlendStateCreateInfo color_blend_state = Vulkan::Colors::create_color_blend_state();
    const VkPipelineDepthStencilStateCreateInfo depth_stencil = Vulkan::Depth::create_depth_stencil();
    const VkPipelineDynamicStateCreateInfo dynamic_states = Vulkan::Pipeline::create_dynamic_states();
    const VkPipelineMultisampleStateCreateInfo multisampling_state = Vulkan::Pipeline::create_multisampling_state(samples_count);
    const VkPipelineRasterizationStateCreateInfo rasterization_state = Vulkan::Pipeline::create_rasterization_state();
    const VkPipelineVertexInputStateCreateInfo vertex_input_state = Vulkan::Vertex::create_vertex_input_state();
    const VkPipelineViewportStateCreateInfo viewport_state = Vulkan::Pipeline::create_viewport_state();

    const Vulkan::Pipeline::pipeline_layout_handler pipeline_layout(descriptor_set_layout.get(), logical_device.get());
    const Vulkan::Pipeline::graphics_pipeline_handler graphics_pipeline(assembly_state, color_blend_state, depth_stencil, dynamic_states, logical_device.get(), multisampling_state, pipeline_layout.get(), shaders_stages, rasterization_state, render_pass.get(), vertex_input_state, viewport_state);

    bool running = true;
    size_t frame = 0;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                running = false;

            if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat)
                Utils::Logs::log("New key down: " + std::string(SDL_GetKeyName(event.key.key)), true);

            if (event.type == SDL_EVENT_KEY_UP)
                Utils::Logs::log("New key up: " + std::string(SDL_GetKeyName(event.key.key)), true);
        }

        const std::string draw_output = Vulkan::Render::draw_frame(command_buffers, descriptor_sets.get(), extent, fences.get(), frame, framebuffers.get(), graphics_pipeline.get(), graphics_queue, image_available_semaphores, index_buffer.get(), indices, logical_device.get(), pipeline_layout.get(), present_queue, render_finished_semaphores, render_pass.get(), scissor, swapchain.get(), texture_image_views.get(), uniform_buffers.get(), vertex_buffer.get(), viewport);
        frame = (frame + 1) % images_count;

        if (draw_output == "recreate")
        {
            const std::string recreate_output = Vulkan::Swapchain::recreate_swapchain(color_resources, command_pool.get(), depth_resources, extent, framebuffers, graphics_family_index, graphics_queue, image_available_semaphores, logical_device.get(), physical_device, present_mode, present_family_index, render_finished_semaphores, render_pass.get(), samples_count, semaphores, surface_format, swapchain, swapchain_images_views, vulkan_surface.get(), window);

            if (recreate_output == "exit")
                running = false;
        }

        Project::run_game_loop();
    }

    Utils::Logs::log("Waiting for device and queues to idle.. ", true);

    vkDeviceWaitIdle(logical_device.get());
    vkQueueWaitIdle(graphics_queue);
    vkQueueWaitIdle(present_queue);

    Utils::Logs::log("Done! Shutting down..", true);
}
