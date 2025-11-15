#include "vertex.handler.hpp"

#include <vulkan/vulkan.h>
#include <cstdint>
#include <utility>

#ifndef VULKAN_VERTEX_BUFFER_HPP
#define VULKAN_VERTEX_BUFFER_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

uint32_t find_memory_type
(
    const uint32_t &type_filter,
    const VkPhysicalDeviceMemoryProperties &memory_properties,
    const VkMemoryPropertyFlags &properties
);

std::pair<VkBuffer, VkDeviceMemory> create_vulkan_vertex_buffer
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    std::vector<Vertex> &vertices
);

void destroy_vulkan_vertex_buffer
(
    const VkDevice &logical_device,
    VkBuffer &vertex_buffer,
    VkDeviceMemory &vertex_buffer_memory
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_VertexBuffer
{

public:
    // Constructor.
    Vulkan_VertexBuffer
    (
        const VkDevice &logical_device,
        const VkPhysicalDevice &physical_device,
        const VkCommandPool &command_pool,
        const VkQueue &graphics_queue,
        std::vector<Vertex> &vertices
    );

    // Destructor.
    ~Vulkan_VertexBuffer();

    VkBuffer get() const;

    // Prevent data duplication.
    Vulkan_VertexBuffer(const Vulkan_VertexBuffer&) = delete;
    Vulkan_VertexBuffer &operator = (const Vulkan_VertexBuffer&) = delete;

private:
    // We declare the members of the class to store.
    VkBuffer vertex_buffer = VK_NULL_HANDLE;
    VkDeviceMemory vertex_buffer_memory = VK_NULL_HANDLE;
    VkDevice logical_device = VK_NULL_HANDLE;

};

#endif
