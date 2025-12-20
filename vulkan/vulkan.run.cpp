#include "vulkan.run.hpp"

#include "../config/engine.config.hpp"
#include "../logs/logs.handler.hpp"
#include "../game/game.main.hpp"
#include "buffers/buffer.index.hpp"
#include "colors/color.attachment.hpp"
#include "colors/color.resources.hpp"
#include "commands/command.buffers.hpp"
#include "commands/command.pool.hpp"
#include "core/vulkan.instance.hpp"
#include "core/vulkan.surface.hpp"
#include "core/validation.layers.hpp"
#include "core/vulkan.extensions.hpp"
#include "depth/depth.attachments.hpp"
#include "depth/depth.resources.hpp"
#include "descriptors/descriptor.set.layout.hpp"
#include "descriptors/descriptor.pool.hpp"
#include "descriptors/descriptor.sets.hpp"
#include "device/physical.device.hpp"
#include "device/logical.device.hpp"
#include "pipeline/pipeline.input.assembly.state.hpp"
#include "pipeline/pipeline.dynamic.states.hpp"
#include "pipeline/graphics.pipeline.hpp"
#include "pipeline/pipeline.multisampling.hpp"
#include "pipeline/pipeline.layout.hpp"
#include "pipeline/pipeline.rasterization.hpp"
#include "pipeline/rect2d.scissor.hpp"
#include "pipeline/viewport.hpp"
#include "queues/graphics.queue.hpp"
#include "queues/present.queue.hpp"
#include "queues/queues.handler.hpp"
#include "render/draw.frames.hpp"
#include "render/multisampling.hpp"
#include "render/render.framebuffers.hpp"
#include "render/render.pass.hpp"
#include "render/sync/render.sync.fences.hpp"
#include "render/sync/render.sync.semaphores.hpp"
#include "shaders/shader.modules.hpp"
#include "shaders/shader.stages.hpp"
#include "swapchain/swapchain.data.queries.hpp"
#include "swapchain/swapchain.images.hpp"
#include "swapchain/swapchain.data.selection.hpp"
#include "swapchain/swapchain.recreation.hpp"
#include "swapchain/swapchain.handler.hpp"
#include "textures/texture.image.buffers.hpp"
#include "textures/texture.image.views.hpp"
#include "textures/texture.images.handler.hpp"
#include "textures/texture.images.loader.hpp"
#include "textures/texture.sampler.hpp"
#include "uniform/uniform.buffers.hpp"
#include "vertex/vertex.buffer.hpp"
#include "vertex/vertex.input.state.hpp"
#include "vertex/models/models.loader.hpp"

#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>

// Run the game using Vulkan as the graphics API.
void run_using_vulkan
(
    SDL_Window* window,
    int &gpu_index
)
{
    std::vector<const char*> layers;

    // Only enable the Vulkan validation layers in debug mode.
    if constexpr (EngineConfig::DEBUG_MODE)
        layers.push_back("VK_LAYER_KHRONOS_validation");

    // Verify that this device handles the required layers.
    check_vulkan_validation_layers_compatibility(layers);

    const Vulkan_Instance vulkan_instance(layers);                       // Start Vulkan.
    const Vulkan_Surface vulkan_surface (vulkan_instance.get(), window); // Create the surface that we are going to use for rendering.

    const VkPhysicalDevice physical_device = select_physical_device(vulkan_instance.get(), gpu_index); // Select a compatible device with rendering capabilities.
    const VkSampleCountFlagBits samples_count = get_max_sample_count(physical_device);

    // Retrieve the swap chain capabilities of our selected physical device.
    const VkSurfaceCapabilitiesKHR swapchain_capabilities = get_vulkan_swapchain_capabilities(physical_device, vulkan_surface.get());
    VkExtent2D extent = select_vulkan_swapchain_extent_resolution(swapchain_capabilities, window); // Select the swap chain resolution.

    // Query and select the best surface format and present mode of the selected physical device.
    const std::vector<VkSurfaceFormatKHR> swapchain_surface_formats = get_vulkan_swapchain_surface_formats(physical_device, vulkan_surface.get());
    const std::vector<VkPresentModeKHR> swapchain_present_modes = get_vulkan_swapchain_present_modes(physical_device, vulkan_surface.get());
    const VkSurfaceFormatKHR surface_format = select_best_vulkan_swapchain_surface_format(swapchain_surface_formats);
    const VkPresentModeKHR present_mode = select_best_vulkan_swapchain_present_mode(swapchain_present_modes);

    // Determine the amount of images that we can produce at once with our swap chain.
    uint32_t images_count = swapchain_capabilities.minImageCount + 1;

    if (swapchain_capabilities.maxImageCount > 0 && images_count > swapchain_capabilities.maxImageCount)
    {
        images_count = swapchain_capabilities.maxImageCount; // Select the maximum images count instead.
        error_log("Fixed the images count which was higher than the swapchain capabilities: " + std::to_string(images_count) + " > " + std::to_string(swapchain_capabilities.maxImageCount) + ".");
    }

    // Retrieve all queues available on the physical device.
    // Then retrieve the graphics and present queue families.
    const std::vector<VkQueueFamilyProperties> queue_families_list = get_queue_families(physical_device);
    const uint32_t graphics_family_index = get_graphics_family_index(queue_families_list);
    const uint32_t present_family_index = get_present_family_index(queue_families_list, physical_device, vulkan_surface.get());

    // Set their indexes as required.
    std::vector<uint32_t> required_queue_indexes = { graphics_family_index, present_family_index };

    // List the Vulkan extensions that we are going to use.
    const std::vector<const char*> required_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    check_vulkan_extensions_support(physical_device, required_extensions);

    // Make the queues' create info for the logical device.
    const std::vector<VkDeviceQueueCreateInfo> queues_create_info = make_queues_create_info(physical_device, required_queue_indexes, 1);

    // Create the logical device which handles rendering, queues and extensions.
    const Vulkan_LogicalDevice logical_device(physical_device, queues_create_info, required_extensions);

    // Create the color resources for multisampling.
    Vulkan_ColorResources color_resources(physical_device, logical_device.get(), extent, surface_format.format, samples_count);

    // Retrieve the queues that have been created alongside the logical device.
    VkQueue graphics_queue, present_queue;
    vkGetDeviceQueue(logical_device.get(), graphics_family_index, 0, &graphics_queue);
    vkGetDeviceQueue(logical_device.get(), present_family_index, 0, &present_queue);

    // Create the swap chain which handles the chain of images during rendering.
    Vulkan_Swapchain swapchain
    (
        logical_device.get(),
        swapchain_capabilities,
        present_mode,
        vulkan_surface.get(),
        surface_format,
        extent,
        graphics_family_index,
        present_family_index,
        images_count
    );

    // Retrieve all swap chain images and create their views.
    const std::vector<VkImage> swapchain_images = get_vulkan_swapchain_images(logical_device.get(), swapchain.get());
    Vulkan_SwapchainImageViews swapchain_images_views(logical_device.get(), swapchain_images, surface_format.format);

    // Load and create modules and stages for the shaders.
    const Vulkan_ShadersModules shaders_modules(logical_device.get());
    const std::vector<VkPipelineShaderStageCreateInfo> shaders_stages = create_vulkan_shader_stages(shaders_modules.get());

    // Prepare create info for graphics pipeline's components.
    const VkPipelineDynamicStateCreateInfo dynamic_states = create_vulkan_dynamic_states(); // Allow to modify states without having to recreate the graphics pipeline.
    const VkPipelineVertexInputStateCreateInfo vertex_input_state = create_vulkan_vertex_input_state(); // Handle inputs in the vertex shader files.
    const VkPipelineInputAssemblyStateCreateInfo assembly_input_state = create_vulkan_assembly_input_state(); // Define how we assemble objects.
    const VkPipelineRasterizationStateCreateInfo rasterization_state = create_vulkan_rasterization_state(); // Handle the conversion from geometry to pixels.
    const VkPipelineMultisampleStateCreateInfo multisampling_state = create_vulkan_multisampling_state(samples_count); // Anti-aliasing.

    // Define the region of the Vulkan surface that will be affected by the rendering.
    const VkPipelineViewportStateCreateInfo viewport_state = create_viewport_state();
    const VkViewport viewport = create_vulkan_viewport(extent);
    const VkRect2D scissor = create_vulkan_scissor(extent);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    load_3d_models(vertices, indices);

    const Vulkan_CommandPool command_pool(logical_device.get(), graphics_family_index); // Handle command buffers memory.
    const std::vector<VkCommandBuffer> command_buffers = create_vulkan_command_buffers(logical_device.get(), command_pool.get(), images_count); // Store sent commands.
    const Vulkan_VertexBuffer vertex_buffer(logical_device.get(), physical_device, command_pool.get(), graphics_queue, vertices); // Handle the vertex shader data.
    const Vulkan_IndexBuffer index_buffer(logical_device.get(), physical_device, command_pool.get(), graphics_queue, vertices, indices); // Handle the shader data indexes.
    const Vulkan_UniformBuffers uniform_buffers(logical_device.get(), physical_device, command_pool.get(), graphics_queue, images_count); // Handle data passed to shaders.

    // Depth management.
    Vulkan_DepthResources depth_resources(physical_device, logical_device.get(), command_pool.get(), graphics_queue, extent, samples_count);
    const VkAttachmentDescription depth_attachment = create_depth_attachment(physical_device, samples_count);
    const VkAttachmentReference depth_attachment_reference = create_depth_attachment_reference();

    const VkAttachmentDescription color_attachment = create_vulkan_color_attachment(surface_format.format, samples_count);
    const Vulkan_RenderPass render_pass(logical_device.get(), color_attachment, depth_attachment, depth_attachment_reference, surface_format); // Define the way we use color attachments for rendering.
    Vulkan_Framebuffers framebuffers(logical_device.get(), swapchain_images_views.get(), color_resources.get().color_image_view, depth_resources.get().image_view, extent, render_pass.get()); // Store the image views in buffers.

    const Vulkan_Fence fences(logical_device.get(), images_count); // Handle CPU/GPU synchronisation.
    Vulkan_Semaphores semaphores(logical_device.get(), (images_count * 2)); // Image retrieve and rendering synchronisation.

    const int fences_count = fences.get().size();
    const int semaphores_count = semaphores.get().size();

    if (fences_count != semaphores_count / 2)
    {
        fatal_error_log("The amount of semaphores doesn't match the amount of fences: " + std::to_string(fences.get().size()) + " != " + std::to_string(semaphores_count / 2) + ".");
    }

    if (semaphores_count % 2 != 0)
    {
        fatal_error_log("The amount of semaphores (" + std::to_string(semaphores_count) + ") isn't even!");
    }

    // Control when an image is ready to be drawn.
    std::vector<VkSemaphore> image_available_semaphores;
    image_available_semaphores.reserve(semaphores_count / 2);

    // Control when an image rendering is done and ready to be shown.
    std::vector<VkSemaphore> render_finished_semaphores;
    render_finished_semaphores.reserve(semaphores_count / 2);

    int i = 0;

    for (const VkSemaphore &semaphore : semaphores.get())
    {
        i++;

        if (semaphore == VK_NULL_HANDLE)
        {
            fatal_error_log("Semaphore #" + std::to_string(i) + "/" + std::to_string(semaphores_count) + " not valid!");
        }

        // Half of the semaphores goes to a list, the other half to another.
        if (semaphores_count / 2 < i)
        {
            image_available_semaphores.emplace_back(semaphore);
        }
        else render_finished_semaphores.emplace_back(semaphore);
    }

    const Vulkan_LoadedTextureImages loaded_texture_images; // Load images from the textures folder.
    Vulkan_TextureImageBuffers texture_image_buffers(logical_device.get(), physical_device, loaded_texture_images.get()); // Create some buffers for them.

    const Vulkan_TextureImages texture_images
    (
        logical_device.get(),
        physical_device,
        command_pool.get(),
        graphics_queue,
        loaded_texture_images.get(),
        texture_image_buffers
    );

    const Vulkan_TextureImageViews texture_image_views(logical_device.get(), texture_images.get()); // Make views for the texture images.
    const Vulkan_TextureSampler texture_sampler(physical_device, logical_device.get());

    const Vulkan_DescriptorSetLayout descriptor_set_layout(logical_device.get(), texture_image_views.get());      // Describe the shader layouts.
    const Vulkan_DescriptorPool descriptor_pool(logical_device.get(), images_count, texture_images.get().size()); // Descriptor sets allocator.

    // Bind our uniform buffers to the shaders.
    const std::vector<VkDescriptorSet> descriptor_sets = create_vulkan_descriptor_sets
    (
        logical_device.get(),
        images_count,
        descriptor_set_layout.get(),
        descriptor_pool.get(),
        uniform_buffers.get(),
        texture_image_views.get(),
        texture_sampler.get()
    );

    const Vulkan_PipelineLayout pipeline_layout(logical_device.get(), descriptor_set_layout.get());

    // Assemble all rendering, stages and states components in one single pipeline.
    const Vulkan_GraphicsPipeline graphics_pipeline
    (
        logical_device.get(),
        shaders_stages,
        vertex_input_state,
        assembly_input_state,
        viewport_state,
        rasterization_state,
        multisampling_state,
        pipeline_layout.get(),
        render_pass.get(),
        dynamic_states
    );

    bool running = true;
    size_t frame = 0; // Targeted frame by the drawing function.
    SDL_Event event;  // Window events listener.

    // Main app loop.
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            {
                running = false; // The game will stop running at the next frame.
            }
        }

        // Try to render and draw the frame onto the window.
        const std::string draw_output = draw_frame
        (
            logical_device.get(),
            fences.get(),
            swapchain.get(),
            image_available_semaphores,
            render_finished_semaphores,
            command_buffers, extent,
            framebuffers.get(),
            render_pass.get(),
            graphics_pipeline.get(),
            viewport,
            scissor,
            graphics_queue,
            present_queue,
            frame,
            vertex_buffer.get(),
            index_buffer.get(),
            uniform_buffers.get(),
            pipeline_layout.get(),
            descriptor_sets,
            texture_image_views.get(),
            indices
        );

        // Passing to the next frame.
        // Example: 0 -> 1 -> 2 -> 0 -> 1 -> 2 -> 0...
        frame = (frame + 1) % images_count;

        // Recreate the swap chain as the draw function requested it.
        if (draw_output == "recreate")
        {
            const std::string recreate_output = recreate_vulkan_swapchain
            (
                logical_device.get(),
                vulkan_surface.get(),
                physical_device,
                surface_format,
                present_mode,
                graphics_family_index,
                present_family_index,
                render_pass.get(),
                command_pool.get(),
                graphics_queue,
                samples_count,
                window,
                swapchain,
                swapchain_images_views,
                framebuffers,
                depth_resources,
                color_resources,
                extent,
                semaphores,
                image_available_semaphores,
                render_finished_semaphores
            );

            if (recreate_output == "exit")
            {
                running = false; // User requested to stop the app.
            }
        }

        // Running the game main code at each frame.
        run_game_loop();
    }

    log("Waiting for the device and queues activities to end before leaving..");

    // Wait for the rendering tools to idle.
    vkDeviceWaitIdle(logical_device.get());
    vkQueueWaitIdle(graphics_queue);
    vkQueueWaitIdle(present_queue);

    log("Resources are idling! Exiting..");
}
