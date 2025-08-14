#include "wrapper.hpp"

#include "../../logs/terminal.hpp"
#include "../../tools/text_format.hpp"

#include <vulkan/vulkan.h>

// Begin a command buffer for one single usage.
VkCommandBuffer begin_one_time_vulkan_command_buffer
(
    const VkDevice &logical_device,
    const VkCommandPool &command_pool
)
{
    log(" > Creating and beginning a one time command buffer..");

    // Info about the buffer allocation.
    VkCommandBufferAllocateInfo allocation_info {};
    allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocation_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; // Select the primary command buffer (only one existing yet).
    allocation_info.commandPool = command_pool;              // Pass the instance command pool.
    allocation_info.commandBufferCount = 1;                  // Amount of command buffers to pass.

    VkCommandBuffer command_buffer = VK_NULL_HANDLE;
    VkResult buffer_allocation = vkAllocateCommandBuffers(logical_device, &allocation_info, &command_buffer); // Try to allocate the command buffer.

    if (buffer_allocation != VK_SUCCESS)
    {
        fatal_error_log("Command buffer beginning failed! Command buffer allocation returned error code " + std::to_string(buffer_allocation) + ".");
    }

    if (!command_buffer || command_buffer == VK_NULL_HANDLE)
    {
        fatal_error_log("Command buffer beginning failed! Command buffer allocation output \"" + force_string(command_buffer) + "\" is not valid!");
    }

    // Command buffer start info.
    VkCommandBufferBeginInfo begin_info {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; // Optimize the buffer for a one time submit.

    // Try to start to record commands into the command buffer.
    VkResult buffer_launch = vkBeginCommandBuffer(command_buffer, &begin_info);

    if (buffer_launch != VK_SUCCESS)
    {
        fatal_error_log("The command buffer beginning returned error code " + std::to_string(buffer_launch) + ".");
    }

    log(" > Command buffer " + force_string(command_buffer) + " created and began successfully!");
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
    VkResult buffer_end = vkEndCommandBuffer(command_buffer); // Try to stop the buffer recording.

    if (buffer_end != VK_SUCCESS)
    {
        fatal_error_log("Buffer ending failed! The command buffer end returned error code " + std::to_string(buffer_end) + ".");
    }

    // Submit info for the command buffer recording.
    VkSubmitInfo submit_info {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;            // Number of command buffers to submit.
    submit_info.pCommandBuffers = &command_buffer; // Pointer to the command buffer.

    // Try to submit the command buffer to our graphics queue.
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
