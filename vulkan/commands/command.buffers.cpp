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

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Command buffers creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (command_pool == VK_NULL_HANDLE)
    {
        fatal_error_log("Commands buffers creation failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");
    }

    if (images_count < 1)
    {
        fatal_error_log("Command buffers creation failed! The amount of images provided (" + std::to_string(images_count) + ") is not valid!");
    }

    std::vector<VkCommandBuffer> command_buffers(images_count);

    VkCommandBufferAllocateInfo allocation_info
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = command_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, // Set the level of the buffer to primary.
        .commandBufferCount = images_count        // Amount of command buffers to allocate.
    };

    const VkResult buffer_allocation = vkAllocateCommandBuffers(logical_device, &allocation_info, command_buffers.data());

    if (buffer_allocation != VK_SUCCESS)
    {
        fatal_error_log("Command buffers allocation returned error code " + std::to_string(buffer_allocation) + ".");
    }

    log(std::to_string(images_count) + " command buffers were created successfully!");
    return command_buffers;
}
