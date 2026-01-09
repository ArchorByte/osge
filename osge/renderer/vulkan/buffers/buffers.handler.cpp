#include "vulkan.buffers.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>

/*
    Create a buffer.
    Warning: There is no class that will automatically destroy this buffer, you have to set one up yourself for memory safety reasons.

    Tasks:
        1) Verify the parameters.
        2) Create the buffer.

    Parameters:
        - buffer                  / VkBuffer              / Buffer to create.
        - buffer_memory           / VkDeviceMemory        / Memory of the buffer to create.
        - buffer_size             / VkDeviceSize          / Size of the buffer to create.
        - logical_device          / VkDevice              / Logical device of the Vulkan instance.
        - physical_device         / VkPhysicalDevice      / Physical device used to run Vulkan.
        - usage_flags             / VkBufferUsageFlags    / Usage flags of the buffer.

    Returns:
        No object returned.
*/
void Vulkan::Buffers::create_buffer
(
    VkBuffer &buffer,
    VkDeviceMemory &buffer_memory,
    const VkDeviceSize &buffer_size,
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkBufferUsageFlags &usage_flags
)
{
    log("Creating a buffer..");

    if (buffer_size < 1)
        fatal_error_log("Buffer creation failed! The buffer size provided (" + std::to_string(buffer_size) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        fatal_error_log("Buffer creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");

    if (physical_device == VK_NULL_HANDLE)
        fatal_error_log("Buffer creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");

    const VkBufferCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = buffer_size,
        .usage = usage_flags,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE // Disallow buffer sharing.
    };

    buffer = VK_NULL_HANDLE;
    const VkResult buffer_creation = vkCreateBuffer(logical_device, &create_info, nullptr, &buffer);

    if (buffer_creation != VK_SUCCESS)
        fatal_error_log("Buffer creation returned error code " + std::to_string(buffer_creation) + ".");

    buffer_memory = Vulkan::Buffers::allocate_buffer_memory(buffer, logical_device, physical_device);
    log("Buffer " + force_string(buffer) + " created successfully!");
}



/*
    Cleanly destroy a buffer and free its memory.

    Tasks:
        1) Verify the parameters.
        2) Destroy the buffer.
        3) Free memory.
        4) Replace the objects' addressess.

    Parameters:
        - buffer         / VkBuffer       / Buffer to destroy.
        - buffer_memory  / VkDeviceMemory / Allocated memory of the buffer to destroy.
        - logical_device / VkDevice       / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Vulkan::Buffers::destroy_buffer
(
    VkBuffer &buffer,
    VkDeviceMemory &buffer_memory,
    const VkDevice &logical_device
)
{
    log("Destroying the " + force_string(buffer) + " buffer and freeing its " + force_string(buffer_memory) + " buffer memory!");

    if (buffer == VK_NULL_HANDLE)
    {
        error_log("Buffer destruction failed! The buffer provided (" + force_string(buffer) + ") is not valid!");
        return;
    }

    if (buffer_memory == VK_NULL_HANDLE)
    {
        error_log("Buffer destruction failed! The buffer memory provided (" + force_string(buffer_memory) + ") is not valid!");
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Buffer destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    vkDestroyBuffer(logical_device, buffer, nullptr);
    vkFreeMemory(logical_device, buffer_memory, nullptr);

    buffer = VK_NULL_HANDLE;
    buffer_memory = VK_NULL_HANDLE;

    log("Buffer destroyed and memory freed successfully!");
}
