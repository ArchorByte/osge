#include "buffer.index.hpp"

#include "buffers.handler.hpp"
#include "buffer.copy.hpp"
#include "../vertex/vertex.handler.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <cstring>
#include <utility>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create an index buffer.
std::pair<VkBuffer, VkDeviceMemory> create_vulkan_index_buffer
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue
)
{
    log("Creating an index buffer..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Index buffer creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Index buffer creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (command_pool == VK_NULL_HANDLE)
    {
        fatal_error_log("Index buffer creation failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");
    }

    if (graphics_queue == VK_NULL_HANDLE)
    {
        fatal_error_log("Index buffer creation failed! The graphics queue provided (" + force_string(graphics_queue) + ") is not valid!");
    }

    const VkDeviceSize buffer_size = sizeof(vertices[0]) * vertices.size(); // Calculate the size of the index buffer.
    VkBuffer staging_index_buffer = VK_NULL_HANDLE;
    VkDeviceMemory staging_buffer_memory = VK_NULL_HANDLE;

    // Create the staging buffers to pass its data directly to the GPU.
    create_vulkan_buffer(logical_device, physical_device, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_index_buffer, staging_buffer_memory);
    void* data;

    // Map the buffer memory in the app address space.
    const bool map_memory = vkMapMemory(logical_device, staging_buffer_memory, 0, buffer_size, 0, &data);

    if (!map_memory)
    {
        fatal_error_log("Index buffer creation failed! Memory mapping failed!");
    }

    memcpy(data, indices.data(), (size_t) buffer_size);   // Copy the index data into the map memory.
    vkUnmapMemory(logical_device, staging_buffer_memory); // Unmap the memory once we finished.

    VkBuffer index_buffer = VK_NULL_HANDLE;
    VkDeviceMemory buffer_memory = VK_NULL_HANDLE;

    // Finally create the actual index buffer.
    create_vulkan_buffer(logical_device, physical_device, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, index_buffer, buffer_memory);
    copy_vulkan_buffer_data(logical_device, command_pool, graphics_queue, staging_index_buffer, index_buffer, buffer_size);
    destroy_vulkan_buffer(logical_device, staging_index_buffer, staging_buffer_memory); // Destroy the staging buffer as we no longer need it.

    log("Index buffer " + force_string(index_buffer) + " created successfully!");
    return { index_buffer, buffer_memory };
}

// Destroy an index buffer.
void destroy_vulkan_index_buffer
(
    const VkDevice &logical_device,
    VkBuffer &buffer,
    VkDeviceMemory &buffer_memory
)
{
    log("Destroying the " + force_string(buffer) + " index buffer..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Index buffer destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (buffer == VK_NULL_HANDLE)
    {
        error_log("Index buffer destruction failed! The buffer provided (" + force_string(buffer) + ") is not valid!");
        return;
    }

    if (buffer_memory == VK_NULL_HANDLE)
    {
        error_log("Index buffer destruction failed! The buffer memory provided (" + force_string(buffer_memory) + ") is not valid!");
        return;
    }

    destroy_vulkan_buffer(logical_device, buffer, buffer_memory);
    buffer = VK_NULL_HANDLE;
    buffer_memory = VK_NULL_HANDLE;

    log("Index buffer destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_IndexBuffer::Vulkan_IndexBuffer
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue
) : logical_device(logical_device)
{
    const std::pair buffer_data = create_vulkan_index_buffer(logical_device, physical_device, command_pool, graphics_queue);

    buffer = buffer_data.first;
    buffer_memory = buffer_data.second;
}

// Destructor.
Vulkan_IndexBuffer::~Vulkan_IndexBuffer()
{
    destroy_vulkan_index_buffer(logical_device, buffer, buffer_memory);
}

VkBuffer Vulkan_IndexBuffer::get() const
{
    return buffer;
}
