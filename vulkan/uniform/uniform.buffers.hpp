#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

#ifndef VULKAN_UNIFORM_BUFFER_HPP
#define VULKAN_UNIFORM_BUFFER_HPP

////////////////////////////////////////////////////
//////////////////// Structures ////////////////////
////////////////////////////////////////////////////

struct UniformBufferObject
{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 projection;
};

struct UniformBufferInfo
{
    VkBuffer buffer;
    VkDeviceMemory buffer_memory;
    void* data;
};

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

std::vector<UniformBufferInfo> create_vulkan_uniform_buffers
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const uint32_t &images_count
);

void destroy_vulkan_uniform_buffers
(
    const VkDevice &logical_device,
    std::vector<UniformBufferInfo> &uniform_buffers
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_UniformBuffers
{

public:
    // Constructor.
    Vulkan_UniformBuffers
    (
        const VkDevice &logical_device,
        const VkPhysicalDevice &physical_device,
        const VkCommandPool &command_pool,
        const VkQueue &graphics_queue,
        const uint32_t &images_count
    );

    // Destructor.
    ~Vulkan_UniformBuffers();

    std::vector<UniformBufferInfo> get() const;

    // Prevent data duplication.
    Vulkan_UniformBuffers(const Vulkan_UniformBuffers&) = delete;
    Vulkan_UniformBuffers &operator = (const Vulkan_UniformBuffers&) = delete;

private:
    // We declare the members of the class to store.
    VkDevice logical_device = VK_NULL_HANDLE;
    std::vector<UniformBufferInfo> uniform_buffers;

};

#endif
