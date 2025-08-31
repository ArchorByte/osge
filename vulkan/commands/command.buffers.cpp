#include "command.buffers.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <cstdint>
#include <vector>

// Create a command buffer for each swap chain image.
std::vector<VkCommandBuffer> create_vulkan_command_buffers
(
    const VkDevice &logical_device,
    const VkCommandPool &command_pool,
    const uint32_t &images_count
)
{
    log("Creating " + std::to_string(images_count) + " command buffers..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Command buffers creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (!command_pool || command_pool == VK_NULL_HANDLE)
    {
        fatal_error_log("Commands buffers creation failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");
    }

    if (images_count < 1)
    {
        fatal_error_log("Command buffers creation failed! The amount of images provided (" + std::to_string(images_count) + ") is not valid!");
    }

    std::vector<VkCommandBuffer> command_buffers;

    // Create a command buffer for each image.
    for (int i = 0; i < images_count; i++)
    {
        // Info for the command buffer allocation.
        VkCommandBufferAllocateInfo info {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        info.commandPool = command_pool;              // Pass our command pool.
        info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; // Set this command buffer as primary.
        info.commandBufferCount = 1;                  // Amount of command buffer to allocate.

        VkCommandBuffer command_buffer = VK_NULL_HANDLE;
        VkResult buffer_allocation = vkAllocateCommandBuffers(logical_device, &info, &command_buffer); // Try to create the command buffer.

        if (buffer_allocation != VK_SUCCESS)
        {
            fatal_error_log("Command buffer " + std::to_string(i + 1) + "/" + std::to_string(images_count) + " allocation returned error code " + std::to_string(buffer_allocation) + ".");
        }

        if (!command_buffer || command_buffer == VK_NULL_HANDLE)
        {
            fatal_error_log("Command buffer " + std::to_string(i + 1) + "/" + std::to_string(images_count) + " allocation output \"" + force_string(command_buffer) + "\" is not valid.");
        }

        command_buffers.emplace_back(command_buffer); // Register the command buffer.
        log("- Command buffer #" + std::to_string(i + 1) + "/" + std::to_string(images_count) + " (" + force_string(command_buffer) + ") created successfully!");
    }

    log(std::to_string(images_count) + " command buffers were created successfully!");
    return command_buffers;
}
