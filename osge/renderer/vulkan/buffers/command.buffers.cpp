#include "vulkan.buffers.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <array>
#include <unistd.h>
#include <vector>

/*
    Create a command buffer for each swap chain image.

    Tasks:
        1) Verify the function parameters.
        2) Allocate the command buffers using the command pool.

    Parameters:
        - command_pool   / VkCommandPool / Handles memory allocation of command buffers.
        - images_count   / uint32_t      / Amount of command buffers to create. One for each swap chain image.
        - logical_device / VkDevice      / Logical device of the Vulkan instance.

    Returns:
        A vector list containing all created command buffers.
*/
std::vector<VkCommandBuffer> Buffers::create_command_buffers
(
    const VkCommandPool &command_pool,
    const uint32_t      &images_count,
    const VkDevice      &logical_device
)
{
    Utils::Logs::log("Creating " + std::to_string(images_count) + " command buffers.. ", false);

    if (command_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Command pool invalid.");

    if (images_count < 1)
        Utils::Logs::crash_log("Failed! Images count invalid -> " + std::to_string(images_count) + ".");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    std::vector<VkCommandBuffer> command_buffers(images_count);

    /*
        - sType              / Defines the type of the structure.
        - commandPool        / Defines which command pool we are going to use.
        - level              / Defines the level of the command buffers: primary or secondary.
        - commandBufferCount / Amount of command buffers to make.
    */
    const VkCommandBufferAllocateInfo allocation_info
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = command_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = images_count
    };

    const VkResult buffer_allocation = vkAllocateCommandBuffers(logical_device, &allocation_info, command_buffers.data());

    if (buffer_allocation != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Allocation returned error code -> " + std::to_string(buffer_allocation) + ".");

    Utils::Logs::log("Done!", true);
    return command_buffers;
}



/*
    Create a one-time command buffer.
    Note: There is no class that will automatically destroy this buffer, you have to set one up yourself for safety reasons.

    Tasks:
        1) Verify the function parameters.
        2) Allocate the buffer with the command pool.
        3) Create the command buffer.

    Parameters:
        - command_pool   / VkCommandPool / Handles memory allocation of command buffers.
        - logical_device / VkDevice      / Logical device of the Vulkan instance.

    Returns:
        The one-time command buffer created.
*/
VkCommandBuffer Buffers::create_one_time_command_buffer
(
    const VkCommandPool &command_pool,
    const VkDevice      &logical_device
)
{
    Utils::Logs::log("Creating one-time command buffer.. ", false);

    if (command_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Command pool invalid.");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    /*
        - sType              / Defines the type of the structure.
        - commandPool        / Defines which command pool we are going to use.
        - level              / Defines the level of the command buffer: primary or secondary.
        - commandBufferCount / Amount of command buffers to make.
    */
    const VkCommandBufferAllocateInfo allocation_info
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = command_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    VkCommandBuffer command_buffer = VK_NULL_HANDLE;
    const VkResult buffer_allocation = vkAllocateCommandBuffers(logical_device, &allocation_info, &command_buffer);

    if (buffer_allocation != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Allocation returned error code -> " + std::to_string(buffer_allocation) + ".");

    /*
        - sType / Defines the type of the structure.
        - flags / Defines to Vulkan what we are going to do with this command buffer.
    */
    const VkCommandBufferBeginInfo begin_info
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };

    const VkResult buffer_launch = vkBeginCommandBuffer(command_buffer, &begin_info);

    if (buffer_launch != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Creation returned error code -> " + std::to_string(buffer_launch) + ".");

    Utils::Logs::log("Done! Memory address -> " + Utils::Text::get_memory_address(command_buffer) + ".", true);
    return command_buffer;
}



/*
    Record a command buffer and draw a frame.

    Tasks:
        1) Verify the function parameters.
        2) Start a command buffer.
        3) Start the render pass.
        4) Bind all elements together for rendering.
        5) Draw the frame.
        6) End the command buffer and render pass.

    Parameters:
        - command_buffer      / VkCommandBuffer         / Records drawing commands to submit later.
        - descriptor_sets     / vector<VkDescriptorSet> / Regroups data together.
        - extent              / VkExtent2D              / Resolution of the swap chain.
        - frame               / size_t                  / Current frame that we are going to work on.
        - framebuffers        / vector<VkFramebuffer>   / Connects resources to the render pass.
        - graphics_pipeline   / VkPipeline              / Converts 3D models into 2D frames.
        - image_index         / uint32_t                / Index of the image we are working on.
        - index_buffer        / VkBuffer                / Enables us to reuse vertices to optimize renderings.
        - pipeline_layout     / VkPipelineLayout        / Defines to the graphics pipeline what resources can be accessed.
        - render_pass         / VkRenderPass            / Organizes rendering tasks.
        - scissor             / VkRect2D                / Defines the area where we will put the pixels.
        - texture_image_views / vector<VkImageView>     / Describes how to access and treat the texture images.
        - vertex_buffer       / VkBuffer                / Contails all vertex data.
        - vertex_indices      / vector<uint32_t>        / References vertices stored in the vertex buffer.
        - viewport            / VkViewport              / Defines where the rendering occurs.

    Returns:
        No object returned.
*/
void Buffers::record_command_buffer_and_draw
(
    const VkCommandBuffer              &command_buffer,
    const std::vector<VkDescriptorSet> &descriptor_sets,
    const VkExtent2D                   &extent,
    const size_t                       &frame,
    const std::vector<VkFramebuffer>   &framebuffers,
    const VkPipeline                   &graphics_pipeline,
    const uint32_t                     &image_index,
    const VkBuffer                     &index_buffer,
    const VkPipelineLayout             &pipeline_layout,
    const VkRenderPass                 &render_pass,
    const VkRect2D                     &scissor,
    const std::vector<VkImageView>     &texture_image_views,
    const VkBuffer                     &vertex_buffer,
    const std::vector<uint32_t>        &vertex_indices,
    const VkViewport                   &viewport
)
{
    if (command_buffer == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Frame rendering failed! Command buffer invalid.", true);
        return;
    }

    if (descriptor_sets.size() < 1)
    {
        Utils::Logs::log("Frame rendering failed! No descriptor sets provided.", true);
        return;
    }

    if (frame >= descriptor_sets.size())
    {
        Utils::Logs::log("Frame rendering failed! Frame index is out of bounds -> " + std::to_string(frame) + " >= " + std::to_string(descriptor_sets.size()) + ".", true);
        return;
    }

    if (framebuffers.size() < 1)
    {
        Utils::Logs::log("Frame rendering failed! No frame buffers provided.", true);
        return;
    }

    if (graphics_pipeline == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Frame rendering failed! Graphics pipeline invalid.", true);
        return;
    }

    if (image_index >= framebuffers.size())
    {
        Utils::Logs::log("Frame rendering failed! Image index is out of bounds -> " + std::to_string(image_index) + " >= " + std::to_string(framebuffers.size()) + ".", true);
        return;
    }

    if (index_buffer == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Frame rendering failed! Index buffer invalid.", true);
        return;
    }

    if (vertex_indices.size() < 1)
    {
        Utils::Logs::log("Frame rendering failed! No vertex indices provided.", true);
        return;
    }

    if (pipeline_layout == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Frame rendering failed! Pipeline layout provided invalid.", true);
        return;
    }

    if (render_pass == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Frame rendering failed! Render pass invalid.", true);
        return;
    }

    if (texture_image_views.size() < 1)
    {
        Utils::Logs::log("Frame rendering failed! No texture image views provided.", true);
        return;
    }

    if (vertex_buffer == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Frame rendering failed! Vertex buffer invalid.", true);
        return;
    }

    /*
        sType / Defines the type of the structure.
    */
    VkCommandBufferBeginInfo buffer_begin_info
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
    };

    vkResetCommandBuffer(command_buffer, 0);
    const VkResult buffer_launch = vkBeginCommandBuffer(command_buffer, &buffer_begin_info);

    if (buffer_launch != VK_SUCCESS)
        Utils::Logs::crash_log("Frame rendering failed! Command buffer starting returned error code -> " + std::to_string(buffer_launch) + ".");

    /*
        color        / Defines which color to use when clearing a color image/attachment.
        depthStencil / Defines which color to use when clearing a depth or stencil image/attachment.
    */
    std::array<VkClearValue, 2> clear_values {};
    clear_values[0].color = {{ 0.0f, 0.0f, 0.0f, 1.0f }};
    clear_values[1].depthStencil = { 1.0f, 0 };

    /*
        - sType           / Defines the type of the structure.
        - renderPass      / Defines which render pass we are going to start.
        - framebuffer     / Defines which frame buffer we are going to work with.
        - renderArea      / Defines the area where the render pass is going to work.
            - offset      / Defines the (x;y) starting position for the render pass.
            - extent      / Defines the resolution of the render pass. In this case, we use the swap chain resolution.
        - clearValueCount / Amount of clear values to pass.
        - pClearValues    / Passes the clear values.
    */
    const VkRenderPassBeginInfo pass_begin_info
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

    if (targeted_texture < 0 || targeted_texture >= texture_image_views.size())
    {
        Utils::Logs::log("Warning: Texture #" + std::to_string(targeted_texture) + " was not found. Switched to default texture.", true);
        targeted_texture = 0;
    }

    vkCmdBeginRenderPass(command_buffer, &pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);
    vkCmdBindVertexBuffers(command_buffer, 0, 1, &vertex_buffers, offsets);
    vkCmdBindIndexBuffer(command_buffer, index_buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0, 1, &descriptor_sets[frame], 0, nullptr);
    vkCmdPushConstants(command_buffer, pipeline_layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(int), &targeted_texture);
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);
    vkCmdDrawIndexed(command_buffer, static_cast<uint32_t>(vertex_indices.size()), 1, 0, 0, 0);
    vkCmdEndRenderPass(command_buffer);

    const VkResult buffer_end = vkEndCommandBuffer(command_buffer);

    if (buffer_end != VK_SUCCESS)
        Utils::Logs::crash_log("Frame rendering failed! Command buffer ending returned error code -> " + std::to_string(buffer_end) + ".");
}



/*
    Destroy a command buffer.

    Tasks:
        1) Verify function parameters.
        2) Stop any buffer activity.
        3) End the command buffer.
        4) Set object to null.

    Parameters:
        - command_buffer / VkCommandBuffer / Command buffer to destroy.
        - command_pool   / VkCommandPool   / Handles memory allocation of command buffers.
        - graphics_queue / VkQueue         / Handles all graphics commands and calls.
        - logical_device / VkDevice        / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Buffers::destroy_command_buffer
(
    VkCommandBuffer     &command_buffer,
    const VkCommandPool &command_pool,
    const VkQueue       &graphics_queue,
    const VkDevice      &logical_device
)
{
    Utils::Logs::log("Destroying command buffer (" + Utils::Text::get_memory_address(command_buffer) + ").. ", false);

    if (command_buffer == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Command buffer invalid.", true);
        return;
    }

    if (command_pool == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Command pool invalid.", true);
        return;
    }

    if (graphics_queue == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Graphics queue invalid.", true);
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    const VkResult buffer_end = vkEndCommandBuffer(command_buffer);

    if (buffer_end != VK_SUCCESS)
    {
        Utils::Logs::log("Failed! Activity stopping returned error code -> " + std::to_string(buffer_end) + ".", true);
        return;
    }

    vkQueueWaitIdle(graphics_queue);
    vkFreeCommandBuffers(logical_device, command_pool, 1, &command_buffer);
    command_buffer = VK_NULL_HANDLE;

    Utils::Logs::log("Done!", true);
}
