#include "vulkan.run.hpp"

#include "../config/engine.config.hpp"
#include "../logs/logs.handler.hpp"
#include "../game/game.main.hpp"
#include "buffers/buffer.index.hpp"
#include "commands/command.buffers.hpp"
#include "commands/command.pool.hpp"
#include "core/vulkan.instance.hpp"
#include "core/vulkan.surface.hpp"
#include "core/validation.layers.hpp"
#include "core/vulkan.extensions.hpp"
#include "descriptors/descriptor.set.layout.hpp"
#include "descriptors/descriptor.pool.hpp"
#include "descriptors/descriptor.sets.hpp"
#include "device/physical.device.hpp"
#include "device/logical.device.hpp"
#include "pipeline/pipeline.input.assembly.state.hpp"
#include "pipeline/color.attachment.hpp"
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

#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>

void run_vulkan
(
    SDL_Window* window
)
{
    // We declare the layers that we are going to use.
    std::vector<const char*> layers;

    // Only enable the validation layers in debug mode.
    #ifdef DEBUG_MODE
        layers.push_back("VK_LAYER_KHRONOS_validation");
    #endif

    // Check that this device handles our layers in the list.
    check_vulkan_validation_layers_compatibility(layers);

    // Create the Vulkan instance.
    // Entry point of Vulkan.
    Vulkan_Instance vulkan_instance(layers);

    // Create the Vulkan surface and apply it to our SDL3 window.
    // Required to present frames.
    Vulkan_Surface vulkan_surface (vulkan_instance.get(), window);

    // Select a usable physical device for rendering.
    // Take a physical device (GPU) that supports Vulkan and is capable of rendering.
    VkPhysicalDevice physical_device = select_physical_device(vulkan_instance.get());

    // Retrieve the swap chain capabilities of our selected physical device.
    // Then, using our swap chain capabilities, we select the resolution for the swap chain.
    VkSurfaceCapabilitiesKHR swapchain_capabilities = get_vulkan_swapchain_capabilities(physical_device, vulkan_surface.get());
    VkExtent2D extent = select_vulkan_swapchain_extent_resolution(swapchain_capabilities, window);

    // Retrieve the different swap chain surface formats of our physical device.
    // Then, we select the best one, according to our requirements.
    std::vector<VkSurfaceFormatKHR> swapchain_surface_formats = get_vulkan_swapchain_surface_formats(physical_device, vulkan_surface.get());
    VkSurfaceFormatKHR surface_format = select_best_vulkan_swapchain_surface_format(swapchain_surface_formats);

    // Retrieve the swap chain present modes.
    // Then, we select the best one, according once again, to our requirements.
    std::vector<VkPresentModeKHR> swapchain_present_modes = get_vulkan_swapchain_present_modes(physical_device, vulkan_surface.get());
    VkPresentModeKHR present_mode = select_best_vulkan_swapchain_present_mode(swapchain_present_modes);

    // Determine the amount of images that we can produce at once with our swap chain.
    uint32_t images_count = swapchain_capabilities.minImageCount + 1;

    // Verify that the images count, that we determined just above, isn't beyond the swap chain capabilities.
    if (swapchain_capabilities.maxImageCount > 0 && images_count > swapchain_capabilities.maxImageCount)
    {
        images_count = swapchain_capabilities.maxImageCount; // Select the maximum images count.
        error_log("Fixed the images count which was higher than the swapchain capabilities: " + std::to_string(images_count) + " > " + std::to_string(swapchain_capabilities.maxImageCount) + ".");
    }

    // Retrieve all queues available on the physical device.
    std::vector<VkQueueFamilyProperties> queue_families_list = get_queue_families(physical_device);

    // Retrieve the queue indexes from the query above.
    // We need the graphics queue for rendering and the present queue for presenting.
    uint32_t graphics_family_index = get_graphics_family_index(queue_families_list);
    uint32_t present_family_index = get_present_family_index(queue_families_list, physical_device, vulkan_surface.get());

    // We register the indexes retrieved in a list.
    std::vector<uint32_t> required_queues_indexes;
    required_queues_indexes.push_back(graphics_family_index);
    required_queues_indexes.push_back(present_family_index);

    // List the Vulkan extensions that we are going to use.
    std::vector<const char*> required_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, };

    // Check that the extensions are supported by the physical device.
    check_vulkan_extensions_support(physical_device, required_extensions);

    // Make the queue create info for the logical device.
    std::vector<VkDeviceQueueCreateInfo> queues_create_info = make_queues_create_info(physical_device, required_queues_indexes, 1);

    // Create the logical device.
    // Logical interface to handle rendering, queues and extensions.
    Vulkan_LogicalDevice logical_device(physical_device, queues_create_info, required_extensions);

    // Get the actual graphics queue.
    VkQueue graphics_queue;
    vkGetDeviceQueue(logical_device.get(), graphics_family_index, 0, &graphics_queue);

    // Get the actual present queue.
    VkQueue present_queue;
    vkGetDeviceQueue(logical_device.get(), present_family_index, 0, &present_queue);

    // Create our swap chain.
    // Manager of the chain of images used.
    Vulkan_Swapchain swapchain(logical_device.get(), swapchain_capabilities, present_mode, vulkan_surface.get(), surface_format, extent, graphics_family_index, present_family_index, images_count);

    // Retrieve the images in the swap chain.
    std::vector<VkImage> swapchain_images = get_vulkan_swapchain_images(logical_device.get(), swapchain.get());

    // Create the views for each image retrieved.
    // Allow shaders to read the images.
    Vulkan_SwapchainImageViews swapchain_images_views(logical_device.get(), swapchain_images, surface_format.format);

    // Read the "shaders" folder and create a shader module for each valid shader available.
    Vulkan_ShadersModules shaders_modules(logical_device.get());

    // Create the shaders stages for each shader module.
    // Prepare the shaders info for the graphics pipeline.
    std::vector<VkPipelineShaderStageCreateInfo> shaders_stages = create_vulkan_shader_stages(shaders_modules.get());

    // Create the dynamic state.
    // Allow to modify some states without having to recreate the graphics pipeline.
    VkPipelineDynamicStateCreateInfo dynamic_states = create_vulkan_dynamic_states();

    // Create the Vertex input state.
    // Format of the Vertex data (inputs) passed to the actual shaders.
    VkPipelineVertexInputStateCreateInfo vertex_input_state = create_vulkan_vertex_input_state();

    // Create the assembly input state.
    // Info about how to assemble objects, such as lines or triangles, from our vertices (vertices location: vulkan/vertex/vertex.hpp).
    VkPipelineInputAssemblyStateCreateInfo assembly_input_state = create_vulkan_assembly_input_state();

    // Create the rasterization state.
    // Handle the conversion from geometry to pixels.
    VkPipelineRasterizationStateCreateInfo rasterization_state = create_vulkan_rasterization_state();

    // Create the multisampling state.
    // Anti-aliasing control using several samples per each pixels. It gives a less pixelated image for lowest resolutions.
    VkPipelineMultisampleStateCreateInfo multisampling_state = create_vulkan_multisampling_state();

    // Create the viewport state.
    // We define how we want the image to be displayed and modified in the frame buffer.
    VkPipelineViewportStateCreateInfo viewport_state = create_viewport_state();

    // Create the viewport.
    // Defines the region of the Vulkan surface that will be affected by the rendering.
    VkViewport viewport = create_vulkan_viewport(extent);

    // Create the scissor.
    // Select the region where we put the pixels. Any pixel outside of it won't be drawn!
    VkRect2D scissor = create_vulkan_scissor(extent);

    // Create the color attachment.
    // Description of the format of the color buffer during the rendering process.
    VkAttachmentDescription color_attachment = create_vulkan_color_attachment(surface_format.format);

    // Create the render pass.
    // The render pass defines how  we are going to use the frame buffers attachments for rendering.
    Vulkan_RenderPass render_pass(logical_device.get(), color_attachment);

    // Create the frame buffers.
    // Store the image views that we use in the render pass.
    Vulkan_Framebuffers framebuffers(logical_device.get(), swapchain_images_views.get(), extent, render_pass.get());

    // Create the command pool.
    // Handle the memory for the command buffers.
    Vulkan_CommandPool command_pool(logical_device.get(), graphics_family_index);

    // Create the command buffers.
    // The command buffers store the commands sent to the physical device.
    std::vector<VkCommandBuffer> command_buffers = create_vulkan_command_buffers(logical_device.get(), command_pool.get(), images_count);

    // Create the Vertex buffer.
    // Handle the Vertex shader data.
    Vulkan_VertexBuffer vertex_buffer(logical_device.get(), physical_device, command_pool.get(), graphics_queue);

    // Create an index buffer.
    // Handle the indexes of the shader data.
    Vulkan_IndexBuffer index_buffer(logical_device.get(), physical_device, command_pool.get(), graphics_queue);

    // Create the uniform buffers.
    // Store the data that we are going to pass to the shaders.
    Vulkan_UniformBuffers uniform_buffers(logical_device.get(), physical_device, command_pool.get(), graphics_queue, images_count);

    // Create the fences.
    // Force the CPU to wait for GPU work to finish.
    Vulkan_Fence fences(logical_device.get(), images_count);

    // Create the semaphores.
    // Coordination between the image retrieve and the rendering order.
    Vulkan_Semaphores semaphores(logical_device.get(), (images_count * 2));

    // Load the images from the textures directory.
    Vulkan_LoadedTextureImages loaded_texture_images;

    // Create the buffers for the texture images.
    Vulkan_TextureImageBuffers texture_image_buffers(logical_device.get(), physical_device, loaded_texture_images.get());

    // Create the texture images.
    Vulkan_TextureImages texture_images(logical_device.get(), physical_device, command_pool.get(), graphics_queue, loaded_texture_images.get(), texture_image_buffers);

    // Create the texture image views.
    Vulkan_TextureImageViews texture_image_views(logical_device.get(), texture_images.get());

    // Create the texture sampler.
    Vulkan_TextureSampler texture_sampler(physical_device, logical_device.get());

    // Create the descriptor set layout.
    // Describes the layout of the shader resources such as uniforms or textures.
    Vulkan_DescriptorSetLayout descriptor_set_layout(logical_device.get(), texture_image_views.get());

    // Create the pipeline layout.
    // Layout of our graphics pipeline. Nothing much more to say about it.
    Vulkan_PipelineLayout pipeline_layout(logical_device.get(), descriptor_set_layout.get());

    // Create the graphics pipeline.
    // Assemble all rendering, stages and states components in one single pipeline.
    Vulkan_GraphicsPipeline graphics_pipeline(logical_device.get(), shaders_stages, vertex_input_state, assembly_input_state, viewport_state, rasterization_state, multisampling_state, pipeline_layout.get(), render_pass.get(), dynamic_states);

    // Create the descriptor pool.
    // Descriptor sets allocator.
    Vulkan_DescriptorPool descriptor_pool(logical_device.get(), images_count, texture_images.get().size());

    // Create the descriptor sets.
    // Bind our uniform buffers just above to the shaders.
    std::vector<VkDescriptorSet> descriptor_sets = create_vulkan_descriptor_sets(logical_device.get(), images_count, descriptor_set_layout.get(), descriptor_pool.get(), uniform_buffers.get(), texture_image_views.get(), texture_sampler.get());

    // Check the amount of semaphores and fences.
    // There can't have more or less the double of fences of semaphores.
    if (fences.get().size() != semaphores.get().size() / 2)
    {
        fatal_error_log("The amount of semaphores don't match the amount of fences : " + std::to_string(fences.get().size()) + " != " + std::to_string(semaphores.get().size() / 2) + ".");
    }

    // Separate the semaphores into the two lists below.
    // The image available semaphores control when an image is ready to be drawn.
    // The render finished semaphores control when an image rendering is done and ready to be shown.
    std::vector<VkSemaphore> image_available_semaphores;
    std::vector<VkSemaphore> render_finished_semaphores;
    int i = 0;

    for (const VkSemaphore &semaphore : semaphores.get())
    {
        i++;

        if (semaphore == VK_NULL_HANDLE)
        {
            fatal_error_log("The semaphore #" + std::to_string(i) + "/" + std::to_string(semaphores.get().size()) + " is not valid!");
        }

        // If we passed the middle of the original list, we add the semaphores in the image available list.
        if (semaphores.get().size() / 2 < i)
        {
            image_available_semaphores.emplace_back(semaphore);
            continue;
        }

        render_finished_semaphores.emplace_back(semaphore);
    }

    size_t frame = 0; // Targeted frame by the drawing function.
    bool running = true;
    SDL_Event event;

    // Main loop.
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }

            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            {
                running = false;
            }
        }

        // Render and draw the frame to the window.
        std::string draw_output = draw_frame(logical_device.get(), fences.get(), swapchain.get(), image_available_semaphores, render_finished_semaphores, command_buffers, extent, framebuffers.get(), render_pass.get(), graphics_pipeline.get(), viewport, scissor, graphics_queue, present_queue, frame, vertex_buffer.get(), index_buffer.get(), uniform_buffers.get(), pipeline_layout.get(), descriptor_sets, texture_image_views.get());

        // Passing to the next frame.
        // Example: 0 -> 1 -> 2 -> 0 -> 1 -> 2 -> 0...
        frame = (frame + 1) % images_count;

        // We need to recreate the swap chain because the draw function asked for it.
        if (draw_output == "recreate")
        {
            std::string recreate_output = recreate_vulkan_swapchain(swapchain, swapchain_images_views, framebuffers, logical_device.get(), vulkan_surface.get(), physical_device, surface_format, present_mode, graphics_family_index, present_family_index, render_pass.get(), extent, window, semaphores, image_available_semaphores, render_finished_semaphores);

            if (recreate_output == "exit")
            {
                running = false;
            }
        }

        // Running the game code at each frame.
        run_game_loop();
    }

    // Before we clean everything up, we wait for the resources (device + queues) to be inactive.
    log("Waiting for the device and queues activities to end before leaving..");

    vkDeviceWaitIdle(logical_device.get()); // Wait for the logical device to idle.
    vkQueueWaitIdle(graphics_queue);        // Wait for the graphics queue to idle.
    vkQueueWaitIdle(present_queue);         // Wait for the present queue to idle.

    log("Resources are idle! Exiting..");
}
