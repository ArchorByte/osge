#include "../vertex/vertex.handler.hpp"

#include <vulkan/vulkan.h>
#include <utility>

#ifndef VULKAN_BUFFERS_INDEX_HPP
#define VULKAN_BUFFERS_INDEX_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

std::pair<VkBuffer, VkDeviceMemory> create_vulkan_index_buffer
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    std::vector<Vertex> &vertices,
    std::vector<uint32_t> &indices
);

void destroy_vulkan_index_buffer
(
    const VkDevice &logical_device,
    VkBuffer &buffer,
    VkDeviceMemory &buffer_memory
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_IndexBuffer
{

public:
    // Constructor.
    Vulkan_IndexBuffer
    (
        const VkDevice &logical_device,
        const VkPhysicalDevice &physical_device,
        const VkCommandPool &command_pool,
        const VkQueue &graphics_queue,
        std::vector<Vertex> &vertices,
        std::vector<uint32_t> &indices
    );

    // Destructor.
    ~Vulkan_IndexBuffer();

    VkBuffer get() const;

    // Prevent data duplication.
    Vulkan_IndexBuffer(const Vulkan_IndexBuffer&) = delete;
    Vulkan_IndexBuffer& operator=(const Vulkan_IndexBuffer&) = delete;

private:
    // We declare the members of the class to store.
    VkDevice logical_device = VK_NULL_HANDLE;
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory buffer_memory = VK_NULL_HANDLE;

};

#endif
