#include "uniform.buffers.hpp"

#include "../buffers/buffers.handler.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <utility>
#include <cstdint>
#include <string>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a uniform buffer for each swap chain image.
std::vector<UniformBufferInfo> create_vulkan_uniform_buffers
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const uint32_t &images_count
)
{
    log("Creating " + std::to_string(images_count) + " uniform buffers..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Uniform buffers creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (!physical_device || physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Uniform buffers creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (!command_pool || command_pool == VK_NULL_HANDLE)
    {
        fatal_error_log("Uniform buffers creation failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");
    }

    if (!graphics_queue || graphics_queue == VK_NULL_HANDLE)
    {
        fatal_error_log("Uniform buffers creation failed! The graphics queue provided (" + force_string(graphics_queue) + ") is not valid!");
    }

    if (images_count < 1)
    {
        fatal_error_log("Uniform buffers creation failed! The images count provided (" + std::to_string(images_count) + ") is not valid!");
    }

    std::vector<UniformBufferInfo> output;
    VkDeviceSize buffer_size = sizeof(UniformBufferObject); // Calculate the size needed for the buffer.

    // Create a uniform buffer for each image.
    for (int i = 0; i < images_count; i++)
    {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory buffer_memory = VK_NULL_HANDLE;
        void* data;

        create_vulkan_buffer(logical_device, physical_device, buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, buffer, buffer_memory);
        vkMapMemory(logical_device, buffer_memory, 0, buffer_size, 0, &data); // Map the buffer memory in the app address space.

        UniformBufferInfo info =
        {
            buffer,
            buffer_memory,
            data
        };

        output.emplace_back(info); // Register the buffer.
        log("- Uniform buffer #" + std::to_string(i + 1) + "/" + std::to_string(images_count) + " (" + force_string(buffer) + ") created successfully!");
    }

    log(std::to_string(output.size()) + " uniform buffers created successfully!");
    return output;
}

// Destroy some uniform buffers.
void destroy_vulkan_uniform_buffers
(
    const VkDevice &logical_device,
    std::vector<UniformBufferInfo> &uniform_buffers
)
{
    log("Destroying " + std::to_string(uniform_buffers.size()) + " uniform buffers..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        error_log("Uniform buffers destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (uniform_buffers.size() < 1)
    {
        error_log("Uniform buffers destruction failed! No uniform buffers were provided!");
        return;
    }

    int failed = 0;
    int i = 0;

    // Destroy each uniform buffer in the list.
    for (UniformBufferInfo &uniform_buffer : uniform_buffers)
    {
        i++;

        VkBuffer buffer = uniform_buffer.buffer;                     // Retrieve the buffer.
        VkDeviceMemory buffer_memory = uniform_buffer.buffer_memory; // Retrieve the buffer memory.
        void* data = uniform_buffer.data;                            // Retrieve the buffer data.

        if (!buffer || buffer == VK_NULL_HANDLE)
        {
            error_log("- Warning: Failed to destroy the uniform buffer #" + std::to_string(i) + "/" + std::to_string(uniform_buffers.size()) + "! The uniform buffer provided (" + force_string(buffer) + ") is not valid!");
            failed++;
            continue;
        }

        if (!buffer_memory || buffer_memory == VK_NULL_HANDLE)
        {
            error_log("- Warning: Failed to destroy the uniform buffer #" + std::to_string(i) + "/" + std::to_string(uniform_buffers.size()) + "! The uniform buffer memory provided (" + force_string(buffer_memory) + ") is not valid!");
            failed++;
            continue;
        }

        if (!data)
        {
            error_log("- Warning: Failed to destroy the uniform buffer #" + std::to_string(i) + "/" + std::to_string(uniform_buffers.size()) + "! The uniform buffer data provided (" + force_string(data) + ") is not valid!");
        }

        // Destroy the buffer and dispose of the address.
        vkDestroyBuffer(logical_device, buffer, nullptr);
        buffer = VK_NULL_HANDLE;

        // Free the memory occupied by the former buffer and dispose of the address as well.
        vkFreeMemory(logical_device, buffer_memory, nullptr);
        buffer_memory = VK_NULL_HANDLE;

        log("- Uniform buffer #" + std::to_string(i) + "/" + std::to_string(uniform_buffers.size()) + " destroyed successfully!");
    }

    if (failed > 0) error_log("Warning: " + std::to_string(failed) + " uniform buffers failed to destroy! This might lead to some memory leaks or memory overload.");
    log(std::to_string(uniform_buffers.size() - failed) + "/" + std::to_string(uniform_buffers.size()) + " uniform buffers destroyed successfully!");

    // Free the list.
    uniform_buffers.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_UniformBuffers::Vulkan_UniformBuffers
(
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device,
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const uint32_t &images_count
) : logical_device(logical_device)
{
    uniform_buffers = create_vulkan_uniform_buffers(logical_device, physical_device, command_pool, graphics_queue, images_count);
}

// Destructor.
Vulkan_UniformBuffers::~Vulkan_UniformBuffers()
{
    destroy_vulkan_uniform_buffers(logical_device, uniform_buffers);
}

std::vector<UniformBufferInfo> Vulkan_UniformBuffers::get() const
{
    return uniform_buffers;
}
