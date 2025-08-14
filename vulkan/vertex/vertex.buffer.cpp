#include "buffer.hpp"

#include "vertex.hpp"
#include "../../logs/terminal.hpp"
#include "../buffers/manager.hpp"
#include "../buffers/copy.hpp"
#include "../buffers/index.hpp"
#include "../../tools/text_format.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>
#include <cstring>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a vertex buffer.
std::pair<VkBuffer, VkDeviceMemory> create_vulkan_vertex_buffer
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue
)
{
    log("Creating a vertex buffer..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Vertex buffer creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (!physical_device || physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Vertex buffer creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (!command_pool || command_pool == VK_NULL_HANDLE)
    {
        fatal_error_log("Vertex buffer creation failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");
    }

    if (!graphics_queue || graphics_queue == VK_NULL_HANDLE)
    {
        fatal_error_log("Vertex buffer creation failed! The graphics queue provided (" + force_string(graphics_queue) + ") is not valid!");
    }

    VkBuffer staging_vertex_buffer = VK_NULL_HANDLE;
    VkDeviceMemory staging_buffer_memory = VK_NULL_HANDLE;
    VkDeviceSize buffer_size = sizeof(vertices[0]) * vertices.size(); // Calculate the size needed by the buffer.

    // Create the staging buffer.
    create_vulkan_buffer(logical_device, physical_device, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_vertex_buffer, staging_buffer_memory);
    void* data;

    vkMapMemory(logical_device, staging_buffer_memory, 0, buffer_size, 0, &data); // Map the buffer memory in the app address space.
    memcpy(data, vertices.data(), (size_t) buffer_size);                          // Copy the data from CPU memory to GPU memory to enhance performances.
    vkUnmapMemory(logical_device, staging_buffer_memory);                         // Unmap the memory once we finished.

    VkBuffer vertex_buffer = VK_NULL_HANDLE;
    VkDeviceMemory buffer_memory = VK_NULL_HANDLE;

    // Create the final (vertex) buffer.
    create_vulkan_buffer(logical_device, physical_device, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertex_buffer, buffer_memory);
    copy_vulkan_buffer_data(logical_device, command_pool, graphics_queue, staging_vertex_buffer, vertex_buffer, buffer_size); // Copy the data from the staging buffer to the final buffer.
    destroy_vulkan_buffer(logical_device, staging_vertex_buffer, staging_buffer_memory); // Dispose of the staging buffer.

    log("Vertex buffer " + force_string(vertex_buffer) + " created successfully!");
    return { vertex_buffer, buffer_memory };
}

// Destroy a vertex buffer.
void destroy_vulkan_vertex_buffer
(
    const VkDevice &logical_device,
    VkBuffer &vertex_buffer,
    VkDeviceMemory &vertex_buffer_memory
)
{
    log("Destroying the " + force_string(vertex_buffer) + " vertex buffer..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        error_log("Vertex buffer destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (!vertex_buffer || vertex_buffer == VK_NULL_HANDLE)
    {
        error_log("Vertex buffer destruction failed! The vertex buffer provided (" + force_string(vertex_buffer) + ") is not valid!");
        return;
    }

    if (!vertex_buffer_memory || vertex_buffer_memory == VK_NULL_HANDLE)
    {
        error_log("Vertex buffer destruction failed! The vertex buffer memory provided (" + force_string(vertex_buffer_memory) + ") is not valid!");
    }

    destroy_vulkan_buffer(logical_device, vertex_buffer, vertex_buffer_memory);
    log("Vertex buffer destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_VertexBuffer::Vulkan_VertexBuffer
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue
) : logical_device(logical_device)
{
    std::pair buffer_data = create_vulkan_vertex_buffer(logical_device, physical_device, command_pool, graphics_queue);

    vertex_buffer = buffer_data.first;
    vertex_buffer_memory = buffer_data.second;
}

// Destructor.
Vulkan_VertexBuffer::~Vulkan_VertexBuffer()
{
    destroy_vulkan_vertex_buffer(logical_device, vertex_buffer, vertex_buffer_memory);
}

VkBuffer Vulkan_VertexBuffer::get() const
{
    return vertex_buffer;
}
