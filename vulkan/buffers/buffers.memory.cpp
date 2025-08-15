#include "buffers.memory.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <cstdint>

// Find the index of a specific memory type.
uint32_t find_memory_type
(
    const uint32_t &type_filter,
    const VkPhysicalDeviceMemoryProperties &memory_properties,
    const VkMemoryPropertyFlags &property_flags
)
{
    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++)
    {
        // If the filter and the properties flags match, we found the memory type index.
        if ((type_filter & (1 << i)) && (memory_properties.memoryTypes[i].propertyFlags & property_flags))
        {
            return i;
        }
    }

    fatal_error_log("Memory allocation failed! Failed to find any suitable memory type!");
    return -1; // Useless line because the log above triggers a crash. But without it, the compiler will give unnecessary warnings during compilation.
}

// Allocate some memory to a buffer.
VkDeviceMemory allocate_vulkan_buffer_memory
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkBuffer &buffer
)
{
    log(" > Allocating memory to the " + force_string(buffer) + " buffer..");

    // Fetch memory requirements for the buffer.
    VkMemoryRequirements memory_requirements;
    vkGetBufferMemoryRequirements(logical_device, buffer, &memory_requirements);

    // Retrieve memory types available on the physical device.
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

    // Memory allocation info.
    VkMemoryAllocateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    info.allocationSize = memory_requirements.size; // Pass the required memory amount to allocate.

    // Get the memory type index from our requirements.
    info.memoryTypeIndex = find_memory_type
    (
        memory_requirements.memoryTypeBits,
        memory_properties,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkResult memory_allocation = vkAllocateMemory(logical_device, &info, nullptr, &memory); // Try to allocate the memory.

    if (memory_allocation != VK_SUCCESS)
    {
        fatal_error_log("Buffer memory allocation returned error code " + std::to_string(memory_allocation) + ".");
    }

    if (!memory || memory == VK_NULL_HANDLE)
    {
        fatal_error_log("Buffer memory allocation output \"" + force_string(memory) + "\" is not valid!");
    }

    // Try to bind the memory to the buffer.
    VkResult memory_binding = vkBindBufferMemory(logical_device, buffer, memory, 0);

    if (memory_binding != VK_SUCCESS)
    {
        fatal_error_log("Buffer memory allocation failed! The memory binding returned error code " + std::to_string(memory_binding) + ".");
    }

    log(" > Memory " + force_string(memory) + " allocated successfully!");
    return memory;
}
