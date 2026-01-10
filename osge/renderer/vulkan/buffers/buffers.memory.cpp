#include "vulkan.buffers.hpp"
#include <osge/utils/utils.hpp>
#include <libraries/vulkan/vulkan.h>

/*
    Get the index of a specific memory type.

    Tasks:
        1) Check each memory type available.
        2) Return the index if our requirements are satisfied.

    Parameters:
        - memory_properties / VkPhysicalDeviceMemoryProperties / Properties of the memory.
        - property_flags    / VkMemoryPropertyFlags            / Targeted property flags.
        - type_filter       / uint32_t                         / Targeted memory types.

    Returns:
        The index of the memory type.
*/
uint32_t Vulkan::Buffers::find_memory_type
(
    const VkPhysicalDeviceMemoryProperties &memory_properties,
    const VkMemoryPropertyFlags &property_flags,
    const uint32_t &type_filter
)
{
    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++)
    {
        if ((type_filter & (1 << i)) && (memory_properties.memoryTypes[i].propertyFlags & property_flags))
        {
            return i;
        }
    }

    Utils::Logs::crash_error_log("Failed to get the memory type index! Failed to find any suitable memory type!");
    return -1; // Avoid compiler warnings.
}



/*
    Allocate and bind some memory to a buffer.
    Warning: There is no class that will automatically free the memory, you have to set one up yourself for memory safety reasons.

    Tasks:
        1) Verify the parameters.
        2) Retrieve necessary information about the memory.
        3) Allocate the memory.
        4) Bind the memory to the buffer.

    Parameters:
        - buffer          / VkBuffer         / Targeted buffer.
        - logical_device  / VkDevice         / Logical device of the Vulkan instance.
        - physical_device / VkPhysicalDevice / Physical device used to run Vulkan.

    Returns:
        The buffer memory.
*/
VkDeviceMemory Vulkan::Buffers::allocate_buffer_memory
(
    const VkBuffer &buffer,
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device
)
{
    Utils::Logs::log("Allocating memory to the " + Utils::Text::get_memory_address(buffer) + " buffer..");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Buffer memory allocation failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Buffer memory allocation failed! The physical device provided (" + Utils::Text::get_memory_address(physical_device) + ") is not valid!");

    if (buffer == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Buffer memory allocation failed! The buffer provided (" + Utils::Text::get_memory_address(buffer) + ") is not valid!");

    VkMemoryRequirements memory_requirements;
    vkGetBufferMemoryRequirements(logical_device, buffer, &memory_requirements);

    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

    VkMemoryAllocateInfo allocation_info
    {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memory_requirements.size,
        .memoryTypeIndex = Vulkan::Buffers::find_memory_type(memory_properties, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
    };

    VkDeviceMemory buffer_memory = VK_NULL_HANDLE;
    const VkResult memory_allocation = vkAllocateMemory(logical_device, &allocation_info, nullptr, &buffer_memory);

    if (memory_allocation != VK_SUCCESS)
        Utils::Logs::crash_error_log("Buffer memory allocation returned error code " + std::to_string(memory_allocation) + ".");

    const VkResult memory_binding = vkBindBufferMemory(logical_device, buffer, buffer_memory, 0);

    if (memory_binding != VK_SUCCESS)
        Utils::Logs::crash_error_log("Buffer memory allocation failed! The memory binding returned error code " + std::to_string(memory_binding) + ".");

    Utils::Logs::log("Memory " + Utils::Text::get_memory_address(buffer_memory) + " allocated successfully!");
    return buffer_memory;
}
