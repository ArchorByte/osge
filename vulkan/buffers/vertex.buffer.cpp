#include "vulkan.buffers.hpp"

#include "vertex.handler.hpp"
#include "models/models.loader.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <cstring>
#include <vector>
#include <vulkan/vulkan.h>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a vertex buffer.
    Note: You should use the pre-made class to handle the vertex buffer rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the parameters.
        2) Create a staging buffer for the data transfer.
        3) Map the memory to put the vertices into the staging buffer.
        4) Create the vertex buffer.
        5) Transfer the data from the staging buffer to the vertex buffer.
        6) End the staging buffer.

    Parameters:
        - command_pool    / VkCommandPool    / Command pool of the Vulkan instance.
        - graphics_queue  / VkQueue          / Graphics queue of the Vulkan instance.
        - logical_device  / VkDevice         / Logical device of the Vulkan instance.
        - physical_device / VkPhysicalDevice / Physical device used to run Vulkan.
        - vertices        / vector<Vertex>   / Targeted vertices.

    Returns:
        A pair containing the vertex buffer and its memory.
*/
std::pair<VkBuffer, VkDeviceMemory> Vulkan::Buffers::create_vertex_buffer
(
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    std::vector<Vertex> &vertices
)
{
    log("Creating a vertex buffer..");

    if (command_pool == VK_NULL_HANDLE)
        fatal_error_log("Vertex buffer creation failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");

    if (graphics_queue == VK_NULL_HANDLE)
        fatal_error_log("Vertex buffer creation failed! The graphics queue provided (" + force_string(graphics_queue) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        fatal_error_log("Vertex buffer creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");

    if (physical_device == VK_NULL_HANDLE)
        fatal_error_log("Vertex buffer creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");

    if (vertices.size() < 1)
        fatal_error_log("Vertex buffer creation failed! No vertices provided!");

    VkBuffer staging_vertex_buffer = VK_NULL_HANDLE;
    VkDeviceMemory staging_buffer_memory = VK_NULL_HANDLE;
    const VkDeviceSize buffer_size = sizeof(vertices[0]) * vertices.size();

    Vulkan::Buffers::create_buffer(staging_vertex_buffer, staging_buffer_memory, buffer_size, logical_device, physical_device, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* data;

    vkMapMemory(logical_device, staging_buffer_memory, 0, buffer_size, 0, &data);
    memcpy(data, vertices.data(), (size_t) buffer_size);
    vkUnmapMemory(logical_device, staging_buffer_memory);

    VkBuffer vertex_buffer = VK_NULL_HANDLE;
    VkDeviceMemory buffer_memory = VK_NULL_HANDLE;

    Vulkan::Buffers::create_buffer(vertex_buffer, buffer_memory, buffer_size, logical_device, physical_device, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    Vulkan::Buffers::copy_buffer_data(buffer_size, command_pool, vertex_buffer,  graphics_queue, logical_device, staging_vertex_buffer);
    Vulkan::Buffers::destroy_buffer(staging_vertex_buffer, staging_buffer_memory, logical_device);

    log("Vertex buffer " + force_string(vertex_buffer) + " created successfully!");
    return { vertex_buffer, buffer_memory };
}



/*
    Cleanly destroy a vertex buffer.

    Tasks:
        1) Verify the parameters.
        2) Destroy the vertex buffer and free its memory.

    Parameters:
        - logical_device       / VkDevice       / Logical device of the Vulkan instance.
        - vertex_buffer        / VkBuffer       / Vertex buffer to destroy.
        - vertex_buffer_memory / VkDeviceMemory / Memory allocated to the vertex buffer.

    Returns:
        No object returned.
*/
void Vulkan::Buffers::destroy_vertex_buffer
(
    const VkDevice &logical_device,
    VkBuffer &vertex_buffer,
    VkDeviceMemory &vertex_buffer_memory
)
{
    log("Destroying the " + force_string(vertex_buffer) + " vertex buffer..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Vertex buffer destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (vertex_buffer == VK_NULL_HANDLE)
    {
        error_log("Vertex buffer destruction failed! The vertex buffer provided (" + force_string(vertex_buffer) + ") is not valid!");
        return;
    }

    if (vertex_buffer_memory == VK_NULL_HANDLE)
    {
        error_log("Vertex buffer destruction failed! The vertex buffer memory provided (" + force_string(vertex_buffer_memory) + ") is not valid!");
        return;
    }

    Vulkan::Buffers::destroy_buffer(vertex_buffer, vertex_buffer_memory, logical_device);
    vertex_buffer = VK_NULL_HANDLE;
    vertex_buffer_memory = VK_NULL_HANDLE;

    log("Vertex buffer destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Vulkan::Buffers::vertex_buffer_handler::vertex_buffer_handler
(
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    std::vector<Vertex> &vertices
) : logical_device(logical_device)
{
    const std::pair buffer_data = Vulkan::Buffers::create_vertex_buffer(command_pool, graphics_queue, logical_device, physical_device, vertices);

    vertex_buffer = buffer_data.first;
    vertex_buffer_memory = buffer_data.second;
}

Vulkan::Buffers::vertex_buffer_handler::~vertex_buffer_handler()
{
    Vulkan::Buffers::destroy_buffer(vertex_buffer, vertex_buffer_memory, logical_device);
}

VkBuffer Vulkan::Buffers::vertex_buffer_handler::get() const
{
    return vertex_buffer;
}
