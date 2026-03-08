#include "vulkan.buffers.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <cstring>
#include <utility>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create an index buffer.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the function parameters.
        2) Create a staging buffer for the data transfer.
        3) Map the memory to put the indices into the staging buffer.
        4) Create the index buffer.
        5) Transfer the data from the staging buffer to the index buffer.
        6) End the staging buffer.

    Parameters:
        - command_pool    / VkCommandPool             / Handles memory allocation of command buffers.
        - graphics_queue  / VkQueue                   / Handles all graphics commands and calls.
        - logical_device  / VkDevice                  / Logical device of the Vulkan instance.
        - physical_device / VkPhysicalDevice          / Physical device used to run this Vulkan instance.
        - vertex_indices  / vector<uint32_t>          / References vertices stored in the vertex buffer.
        - vertices        / vector<Vertex::VertexObj> / Contails all vertex data.

    Returns:
        A pair containing the buffer itself and its memory.
*/
std::pair<VkBuffer, VkDeviceMemory> Buffers::create_index_buffer
(
    const VkCommandPool                  &command_pool,
    const VkQueue                        &graphics_queue,
    const VkDevice                       &logical_device,
    const VkPhysicalDevice               &physical_device,
    const std::vector<uint32_t>          &vertex_indices,
    const std::vector<Vertex::VertexObj> &vertices
)
{
    Utils::Logs::log("Creating an index buffer.. ", false);

    if (command_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Command pool invalid.");

    if (graphics_queue == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Graphics queue invalid.");

    if (vertex_indices.size() < 1)
        Utils::Logs::crash_log("Failed! No vertex indices provided.");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical device invalid.");

    if (vertices.size() < 1)
        Utils::Logs::crash_log("Failed! No vertices provided.");

    const VkDeviceSize buffer_size = sizeof(vertices[0]) * vertices.size();
    VkBuffer staging_index_buffer = VK_NULL_HANDLE;
    VkDeviceMemory staging_buffer_memory = VK_NULL_HANDLE;

    Buffers::create_buffer(staging_index_buffer, staging_buffer_memory, buffer_size, logical_device, physical_device,  VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    void* data;

    vkMapMemory(logical_device, staging_buffer_memory, 0, buffer_size, 0, &data);
    memcpy(data, vertex_indices.data(), (size_t) buffer_size);
    vkUnmapMemory(logical_device, staging_buffer_memory);

    VkBuffer index_buffer = VK_NULL_HANDLE;
    VkDeviceMemory buffer_memory = VK_NULL_HANDLE;

    Buffers::create_buffer(index_buffer, buffer_memory, buffer_size, logical_device, physical_device, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    Buffers::copy_buffer_data(buffer_size, command_pool, index_buffer, graphics_queue, logical_device, staging_index_buffer);
    Buffers::destroy_buffer(staging_index_buffer, staging_buffer_memory, logical_device);

    Utils::Logs::log("Done! Memory address -> " + Utils::Text::get_memory_address(index_buffer) + ".", true);
    return { index_buffer, buffer_memory };
}



/*
    Destroy an index buffer.

    Tasks:
        1) Verify the parameters.
        2) Destroy the index buffer.
        3) Set objects to null.

    Parameters:
        - buffer_memory  / VkDeviceMemory / Memory allocated to the index buffer to destroy.
        - index_buffer   / VkBuffer       / Index buffer to destroy.
        - logical_device / VkDevice       / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Buffers::destroy_index_buffer
(
    VkDeviceMemory &buffer_memory,
    VkBuffer       &index_buffer,
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Destroying index buffer (" + Utils::Text::get_memory_address(index_buffer) + ").. ", false);

    if (index_buffer == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Buffer invalid.", true);
        return;
    }

    if (buffer_memory == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Buffer memory invalid.", true);
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    Buffers::destroy_buffer(index_buffer, buffer_memory, logical_device);
    index_buffer = VK_NULL_HANDLE;
    buffer_memory = VK_NULL_HANDLE;

    Utils::Logs::log("Done!", true);
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Buffers::index_buffer_handler::index_buffer_handler
(
    const VkCommandPool                  &command_pool,
    const VkQueue                        &graphics_queue,
    const VkDevice                       &logical_device,
    const VkPhysicalDevice               &physical_device,
    const std::vector<uint32_t>          &vertex_indices,
    const std::vector<Vertex::VertexObj> &vertices
)
    : logical_device(logical_device)
{
    const std::pair buffer_data = create_index_buffer(command_pool, graphics_queue, logical_device, physical_device, vertex_indices, vertices);

    index_buffer = buffer_data.first;
    buffer_memory = buffer_data.second;
}

Buffers::index_buffer_handler::~index_buffer_handler()
{
    destroy_index_buffer(buffer_memory, index_buffer, logical_device);
}

VkBuffer Buffers::index_buffer_handler::get() const
{
    return index_buffer;
}
