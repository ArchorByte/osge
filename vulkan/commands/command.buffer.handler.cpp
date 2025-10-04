#include "command.buffer.handler.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>

// Begin a command buffer for one single usage.
VkCommandBuffer begin_one_time_vulkan_command_buffer
(
    const VkDevice &logical_device,
    const VkCommandPool &command_pool
)
{
    log(" > Creating and beginning a one time command buffer..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("One time command buffer beginning failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (command_pool == VK_NULL_HANDLE)
    {
        fatal_error_log("One time command buffer beginning failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");
    }

    VkCommandBufferAllocateInfo allocation_info {};
    allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocation_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; // Set the level of the buffer to primary.
    allocation_info.commandPool = command_pool;              // Command pool to use.
    allocation_info.commandBufferCount = 1;                  // Amount of command buffers that we are going to pass.

    VkCommandBuffer command_buffer = VK_NULL_HANDLE;
    VkResult buffer_allocation = vkAllocateCommandBuffers(logical_device, &allocation_info, &command_buffer);

    if (buffer_allocation != VK_SUCCESS)
    {
        fatal_error_log("One time command buffer beginning failed! Command buffer allocation returned error code " + std::to_string(buffer_allocation) + ".");
    }

    if (command_buffer == VK_NULL_HANDLE)
    {
        fatal_error_log("One time command buffer beginning failed! Command buffer allocation output (" + force_string(command_buffer) + ") is not valid!");
    }

    VkCommandBufferBeginInfo buffer_begin_info {};
    buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; // Indicate we are going to use this buffer one time.

    // Try to start to record commands into the command buffer.
    VkResult buffer_launch = vkBeginCommandBuffer(command_buffer, &buffer_begin_info);

    if (buffer_launch != VK_SUCCESS)
    {
        fatal_error_log("The one time command buffer beginning returned error code " + std::to_string(buffer_launch) + ".");
    }

    log(" > One time command buffer " + force_string(command_buffer) + " created and began successfully!");
    return command_buffer;
}

// End a command buffer.
void end_command_buffer
(
    const VkDevice &logical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    VkCommandBuffer &command_buffer
)
{
    log(" > Ending the " + force_string(command_buffer) + " command buffer..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Command buffer ending failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (command_pool == VK_NULL_HANDLE)
    {
        error_log("Command buffer ending failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");
        return;
    }

    if (graphics_queue == VK_NULL_HANDLE)
    {
        error_log("Command buffer ending failed! The graphics queue provided (" + force_string(graphics_queue) + ") is not valid!");
        return;
    }

    if (command_buffer == VK_NULL_HANDLE)
    {
        error_log("Command buffer ending failed! The command buffer provided (" + force_string(command_buffer) + ") is not valid!");
        return;
    }

    VkResult buffer_end = vkEndCommandBuffer(command_buffer);

    if (buffer_end != VK_SUCCESS)
    {
        error_log("Command buffer ending failed with error code " + std::to_string(buffer_end) + ".");
        return;
    }

    VkSubmitInfo submit_info {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;            // Number of command buffers to submit.
    submit_info.pCommandBuffers = &command_buffer; // Pass the command buffer.

    VkResult queue_submit = vkQueueSubmit(graphics_queue, 1, &submit_info, VK_NULL_HANDLE);

    if (queue_submit != VK_SUCCESS)
    {
        fatal_error_log("Buffer ending failed! The queue submit returned error code " + std::to_string(queue_submit) + ".");
    }

    vkQueueWaitIdle(graphics_queue);                                        // Wait for the graphics queue activity to end before freeing the buffer.
    vkFreeCommandBuffers(logical_device, command_pool, 1, &command_buffer); // Free the command buffer.

    command_buffer = VK_NULL_HANDLE;
    log(" > Command buffer ended successfully!");
}
