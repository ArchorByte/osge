#include "buffers.handler.hpp"

#include "buffers.memory.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <cstdint>

// Create a buffer.
// Warning: The buffers created are not associated to any RAII class and won't be destroyed automatically!
void create_vulkan_buffer
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkDeviceSize &buffer_size,
    const VkBufferUsageFlags &usage_flags,
    const VkMemoryPropertyFlags &memory_properties_flags,
    VkBuffer &buffer,
    VkDeviceMemory &buffer_memory
)
{
    log(" > Creating a buffer..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Buffer creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Buffer creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (buffer_size < 1)
    {
        fatal_error_log("Buffer creation failed! The buffer size provided (" + std::to_string(buffer_size) + ") is not valid!");
    }

    // Info for the buffer creation.
    const VkBufferCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = buffer_size,
        .usage = usage_flags,                    // Flags to indicate to Vulkan what we are going to do with that buffer.
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE // Disallow buffer sharing.
    };

    buffer = VK_NULL_HANDLE;
    const VkResult buffer_creation = vkCreateBuffer(logical_device, &create_info, nullptr, &buffer);

    if (buffer_creation != VK_SUCCESS)
    {
        fatal_error_log("Buffer creation returned error code " + std::to_string(buffer_creation) + ".");
    }

    if (buffer == VK_NULL_HANDLE)
    {
        fatal_error_log("Buffer creation output (" + force_string(buffer) + ") is not valid!");
    }

    buffer_memory = allocate_vulkan_buffer_memory(logical_device, physical_device, buffer); // Allocate memory to the buffer.
    log(" > Buffer " + force_string(buffer) + " created successfully!");
}

// Destroy a buffer and free its memory.
void destroy_vulkan_buffer
(
    const VkDevice &logical_device,
    VkBuffer &buffer,
    VkDeviceMemory &buffer_memory
)
{
    log(" > Destroying the " + force_string(buffer) + " buffer and freeing its " + force_string(buffer_memory) + " buffer memory!");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Buffer destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

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

    vkDestroyBuffer(logical_device, buffer, nullptr);
    buffer = VK_NULL_HANDLE;

    vkFreeMemory(logical_device, buffer_memory, nullptr);
    buffer_memory = VK_NULL_HANDLE;

    log(" > Buffer destroyed and memory freed successfully!");
}
