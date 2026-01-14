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
    Note: You should use the pre-made class to handle the index buffer rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the function parameters.
        2) Create a staging buffer for the data transfer.
        3) Map the memory to put the indices into the staging buffer.
        4) Create the index buffer.
        5) Transfer the data from the staging buffer to the index buffer.
        6) End the staging buffer.

    Parameters:
        - command_pool    / VkCommandPool    / Handles the memory allocation of the command buffers.
        - graphics_queue  / VkQueue          / Handles all graphics commands and calls.
        - logical_device  / VkDevice         / Logical device of the Vulkan instance.
        - physical_device / VkPhysicalDevice / Physical device used to run this Vulkan instance.
        - vertex_indices  / vector<uint32_t> / References vertices stored in the vertex buffer.
        - vertices        / vector<Vertex>   / Contails all vertex data.

    Returns:
        A pair containing the buffer itself and its memory.
*/
std::pair<VkBuffer, VkDeviceMemory> Buffers::create_index_buffer
(
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const std::vector<uint32_t> &vertex_indices,
    const std::vector<Vertex> &vertices
)
{
    Utils::Logs::log("Creating an index buffer..");

    if (command_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Index buffer creation failed! The command pool provided (" + Utils::Text::get_memory_address(command_pool) + ") is not valid!");

    if (graphics_queue == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Index buffer creation failed! The graphics queue provided (" + Utils::Text::get_memory_address(graphics_queue) + ") is not valid!");

    if (vertex_indices.size() < 1)
        Utils::Logs::crash_error_log("Index buffer creation failed! No vertex indices provided!");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Index buffer creation failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Index buffer creation failed! The physical device provided (" + Utils::Text::get_memory_address(physical_device) + ") is not valid!");

    if (vertices.size() < 1)
        Utils::Logs::crash_error_log("Index buffer creation failed! No vertices provided!");

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

    Utils::Logs::log("Index buffer " + Utils::Text::get_memory_address(index_buffer) + " created successfully!");
    return { index_buffer, buffer_memory };
}



/*
    Destroy an index buffer.

    Tasks:
        1) Verify the parameters.
        2) Destroy the index buffer.
        3) Get rid of the objects memory addresses.

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
    VkBuffer &index_buffer,
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Destroying the " + Utils::Text::get_memory_address(index_buffer) + " index buffer..");

    if (index_buffer == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Index buffer destruction failed! The buffer provided (" + Utils::Text::get_memory_address(index_buffer) + ") is not valid!");
        return;
    }

    if (buffer_memory == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Index buffer destruction failed! The buffer memory provided (" + Utils::Text::get_memory_address(buffer_memory) + ") is not valid!");
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Index buffer destruction failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");
        return;
    }

    Buffers::destroy_buffer(index_buffer, buffer_memory, logical_device);
    index_buffer = VK_NULL_HANDLE;
    buffer_memory = VK_NULL_HANDLE;

    Utils::Logs::log("Index buffer destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Buffers::index_buffer_handler::index_buffer_handler
(
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const std::vector<uint32_t> &vertex_indices,
    const std::vector<Vertex> &vertices
)
    : logical_device(logical_device)
{
    const std::pair buffer_data = Buffers::create_index_buffer(command_pool, graphics_queue, logical_device, physical_device, vertex_indices, vertices);

    index_buffer = buffer_data.first;
    buffer_memory = buffer_data.second;
}

Buffers::index_buffer_handler::~index_buffer_handler()
{
    Buffers::destroy_index_buffer(buffer_memory, index_buffer, logical_device);
}

VkBuffer Buffers::index_buffer_handler::get() const
{
    return index_buffer;
}
