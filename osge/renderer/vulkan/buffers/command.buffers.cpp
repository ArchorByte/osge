#include "vulkan.buffers.hpp"
#include "osge/utils/utils.hpp"
#include <array>
#include <libraries/vulkan/vulkan.h>
#include <unistd.h>
#include <vector>

/*
    Create a command buffer for each swap chain image.

    Tasks:
        1) Verify the parameters.
        2) Allocate all necessary command buffers.

    Parameters:
        - command_pool   / VkCommandPool / Command pool of the Vulkan instance.
        - images_count   / uint32_t      / Amount of command buffers to create.
        - logical_device / VkDevice      / Logical device of the Vulkan instance.

    Returns:
        A vector list containing all created command buffers.
*/
std::vector<VkCommandBuffer> Vulkan::Buffers::create_command_buffers
(
    const VkCommandPool &command_pool,
    const uint32_t &images_count,
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Creating " + std::to_string(images_count) + " command buffers..");

    if (command_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Commands buffers creation failed! The command pool provided (" + Utils::Text::get_memory_address(command_pool) + ") is not valid!");

    if (images_count < 1)
        Utils::Logs::crash_error_log("Command buffers creation failed! The amount of images provided (" + std::to_string(images_count) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Command buffers creation failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    std::vector<VkCommandBuffer> command_buffers(images_count);

    VkCommandBufferAllocateInfo allocation_info
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = command_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = images_count
    };

    const VkResult buffer_allocation = vkAllocateCommandBuffers(logical_device, &allocation_info, command_buffers.data());

    if (buffer_allocation != VK_SUCCESS)
        Utils::Logs::crash_error_log("Command buffers allocation returned error code " + std::to_string(buffer_allocation) + ".");

    Utils::Logs::log(std::to_string(images_count) + " command buffers were created successfully!");
    return command_buffers;
}



/*
    Create a one-time command buffer.
    Note: There is no class that will automatically destroy this buffer, you have to set one up yourself for safety reasons.

    Tasks:
        1) Verify the parameters.
        2) Allocate the buffer.
        3) Create the buffer.

    Parameters:
        - command_pool   / VkCommandPool / Command pool of the Vulkan instance.
        - logical_device / VkDevice      / Logical device of the Vulkan instance.

    Returns:
        The one-time command buffer.
*/
VkCommandBuffer Vulkan::Buffers::create_one_time_command_buffer
(
    const VkCommandPool &command_pool,
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Creating a one time command buffer..");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("One time command buffer beginning failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    if (command_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("One time command buffer beginning failed! The command pool provided (" + Utils::Text::get_memory_address(command_pool) + ") is not valid!");

    VkCommandBufferAllocateInfo allocation_info
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = command_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    VkCommandBuffer command_buffer = VK_NULL_HANDLE;
    const VkResult buffer_allocation = vkAllocateCommandBuffers(logical_device, &allocation_info, &command_buffer);

    if (buffer_allocation != VK_SUCCESS)
        Utils::Logs::crash_error_log("One time command buffer creation failed! Command buffer allocation returned error code " + std::to_string(buffer_allocation) + ".");

    VkCommandBufferBeginInfo buffer_begin_info
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };

    const VkResult buffer_launch = vkBeginCommandBuffer(command_buffer, &buffer_begin_info);

    if (buffer_launch != VK_SUCCESS)
        Utils::Logs::crash_error_log("The one time command buffer creation returned error code " + std::to_string(buffer_launch) + ".");

    Utils::Logs::log("One time command buffer " + Utils::Text::get_memory_address(command_buffer) + " created successfully!");
    return command_buffer;
}



/*
    Record the command buffer and draw a frame.

    Tasks:
        1) Verify the parameters.
        2) Start a command buffer.
        3) Start the render pass.
        4) Bind all elements together for rendering.
        5) Draw the frame.
        6) End the command buffer and render pass.

    Parameters:
        - command_buffer      / VkCommandBuffer         / Command buffer of the Vulkan instance.
        - descriptor_sets     / vector<VkDescriptorSet> / Descriptor sets of the Vulkan instance.
        - extent              / VkExtent2D              / Extent of the swap chain.
        - frame               / size_t                  / Current frame we are going to work on.
        - framebuffers        / vector<VkFramebuffer>   / Frame buffers of the Vulkan instance.
        - graphics_pipeline   / VkPipeline              / Graphics pipeline of the Vulkan instance.
        - image_index         / uint32_t                / Index of the image we are working on.
        - index_buffer        / VkBuffer                / Index buffer of the Vulkan instance.
        - indices             / vector<uint32_t>        / Vertex indices.
        - pipeline_layout     / VkPipelineLayout        / Pipeline layout of the graphics pipeline.
        - render_pass         / VkRenderPass            / Render pass of the Vulkan instance.
        - scissor             / VkRect2D                / Scissor of the Vulkan instance.
        - texture_image_views / vector<VkImageView>     / Textures.
        - vertex_buffer       / VkBuffer                / Vertex buffer of the Vulkan instance.
        - viewport            / VkViewport              / Viewport of the swap chain.

    Returns:
        No object returned.
*/
void Vulkan::Buffers::record_command_buffer_and_draw
(
    const VkCommandBuffer &command_buffer,
    const std::vector<VkDescriptorSet> descriptor_sets,
    const VkExtent2D &extent,
    const size_t &frame,
    const std::vector<VkFramebuffer> &framebuffers,
    const VkPipeline &graphics_pipeline,
    const uint32_t &image_index,
    const VkBuffer &index_buffer,
    std::vector<uint32_t> indices,
    const VkPipelineLayout &pipeline_layout,
    const VkRenderPass &render_pass,
    const VkRect2D &scissor,
    const std::vector<VkImageView> texture_image_views,
    const VkBuffer &vertex_buffer,
    const VkViewport &viewport
)
{
    if (command_buffer == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Failed to render a frame! The command buffer provided (" + Utils::Text::get_memory_address(command_buffer) + ") is not valid!");
        return;
    }

    if (descriptor_sets.size() < 1)
    {
        Utils::Logs::error_log("Failed to render a frame! No descriptor sets provided!");
        return;
    }

    if (frame >= descriptor_sets.size())
    {
        Utils::Logs::error_log("Failed to render a frame! The frame index provided is out of bounds for the descriptor sets: " + std::to_string(frame) + " >= " + std::to_string(descriptor_sets.size()) + ".");
        return;
    }

    if (framebuffers.size() < 1)
    {
        Utils::Logs::error_log("Failed to render a frame! No frame buffers provided!");
        return;
    }

    if (graphics_pipeline == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Failed to render a frame! The graphics pipeline provided (" + Utils::Text::get_memory_address(graphics_pipeline) + ") is not valid!");
        return;
    }

    if (image_index >= framebuffers.size())
    {
        Utils::Logs::error_log("Failed to render a frame! The image index provided is out of bounds for the frame buffers: " + std::to_string(image_index) + " >= " + std::to_string(framebuffers.size()) + ".");
        return;
    }

    if (index_buffer == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Failed to render a frame! The index buffer provided (" + Utils::Text::get_memory_address(index_buffer) + ") is not valid!");
        return;
    }

    if (indices.size() < 1)
    {
        Utils::Logs::error_log("Failed to render a frame! No indices provided!");
        return;
    }

    if (pipeline_layout == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Failed to render a frame! The pipeline layout provided (" + Utils::Text::get_memory_address(pipeline_layout) + ") is not valid!");
        return;
    }

    if (render_pass == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Failed to render a frame! The render pass provided (" + Utils::Text::get_memory_address(render_pass) + ") is not valid!");
        return;
    }

    if (texture_image_views.size() < 1)
    {
        Utils::Logs::error_log("Failed to render a frame! No texture image views were provided!");
        return;
    }

    if (vertex_buffer == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Failed to render a frame! The vertex buffer provided (" + Utils::Text::get_memory_address(vertex_buffer) + ") is not valid!");
        return;
    }

    VkCommandBufferBeginInfo begin_info { .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    const VkResult buffer_launch = vkBeginCommandBuffer(command_buffer, &begin_info);

    if (buffer_launch != VK_SUCCESS)
        Utils::Logs::crash_error_log("Failed to render a frame! The command buffer start returned error code " + std::to_string(buffer_launch) + ".");

    std::array<VkClearValue, 2> clear_values {};
    clear_values[0].color = {{ 0.0f, 0.0f, 0.0f, 1.0f }};
    clear_values[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo render_pass_begin_info
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = render_pass,
        .framebuffer = framebuffers[image_index],
        .renderArea =
        {
            .offset = { 0, 0 },
            .extent = extent
        },
        .clearValueCount = static_cast<uint32_t>(clear_values.size()),
        .pClearValues = clear_values.data()
    };

    const VkBuffer vertex_buffers = { vertex_buffer };
    const VkDeviceSize offsets[] = { 0 };
    int targeted_texture = 1;

    if (targeted_texture < 0 || targeted_texture > texture_image_views.size())
    {
        Utils::Logs::error_log("Texture #" + std::to_string(targeted_texture) + " not found!");
        targeted_texture = 0;
    }

    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);
    vkCmdBindVertexBuffers(command_buffer, 0, 1, &vertex_buffers, offsets);
    vkCmdBindIndexBuffer(command_buffer, index_buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0, 1, &descriptor_sets[frame], 0, nullptr);
    vkCmdPushConstants(command_buffer, pipeline_layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(int), &targeted_texture); // Apply the texture.
    vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);
    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);
    vkCmdDrawIndexed(command_buffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
    vkCmdEndRenderPass(command_buffer);

    const VkResult buffer_end = vkEndCommandBuffer(command_buffer);

    if (buffer_end != VK_SUCCESS)
        Utils::Logs::crash_error_log("Failed to render a frame! The command buffer end returned error code " + std::to_string(buffer_end) + ".");
}



/*
    Destroy a command buffer.

    Tasks:
        1) Verify the parameters.
        2) Stop any buffer activity.
        3) End the command buffer.
        4) Replace the object address.

    Parameters:
        - command_buffer / VkCommandBuffer / Command buffer to destroy.
        - command_pool   / VkCommandPool   / Command pool of the Vulkan instance.
        - graphics_queue / VkQueue         / Graphics queue of the Vulkan instance.
        - logical_device / VkDevice        / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Vulkan::Buffers::destroy_command_buffer
(
    VkCommandBuffer &command_buffer,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Destroying the " + Utils::Text::get_memory_address(command_buffer) + " command buffer..");

    if (command_buffer == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Command buffer destruction failed! The command buffer provided (" + Utils::Text::get_memory_address(command_buffer) + ") is not valid!");
        return;
    }

    if (command_pool == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Command buffer destruction failed! The command pool provided (" + Utils::Text::get_memory_address(command_pool) + ") is not valid!");
        return;
    }

    if (graphics_queue == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Command buffer destruction failed! The graphics queue provided (" + Utils::Text::get_memory_address(graphics_queue) + ") is not valid!");
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Command buffer destruction failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");
        return;
    }

    const VkResult buffer_end = vkEndCommandBuffer(command_buffer);

    if (buffer_end != VK_SUCCESS)
    {
        Utils::Logs::error_log("Command buffer destruction failed with error code " + std::to_string(buffer_end) + ".");
        return;
    }

    VkSubmitInfo submit_info
    {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &command_buffer
    };

    const VkResult queue_submit = vkQueueSubmit(graphics_queue, 1, &submit_info, VK_NULL_HANDLE);

    if (queue_submit != VK_SUCCESS)
        Utils::Logs::crash_error_log("Command buffer destruction failed! The queue submit returned error code " + std::to_string(queue_submit) + ".");

    vkQueueWaitIdle(graphics_queue);
    vkFreeCommandBuffers(logical_device, command_pool, 1, &command_buffer);
    command_buffer = VK_NULL_HANDLE;

    Utils::Logs::log("Command buffer destroyed successfully!");
}
