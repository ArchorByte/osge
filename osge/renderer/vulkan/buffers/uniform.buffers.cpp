#include "vulkan.buffers.hpp"
#include "osge/utils/utils.hpp"
#include <chrono>
#include <cstring>
#include <libraries/glm/glm.hpp>
#include <libraries/glm/gtc/matrix_transform.hpp>
#include <libraries/vulkan/vulkan.h>
#include <string>
#include <utility>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create the uniform buffers.
    Note: You should use the pre-made class to handle the uniform buffers rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the parameters.
        2) Create a uniform buffer for each image.

    Parameters:
        - command_pool    / VkCommandPool    / Command pool of the Vulkan instance.
        - graphics_queue  / VkQueue          / Graphics queue of the Vulkan instance.
        - image_count     / uint32_t         / Amount of uniform buffers to create.
        - logical_device  / VkDevice         / Logical device of the Vulkan instance.
        - physical_device / VkPhysicalDevice / Physical device used to run Vulkan.

    Returns:
        A vector list containing all created uniform buffers.
*/
std::vector<UniformBufferInfo> Vulkan::Buffers::create_uniform_buffers
(
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const uint32_t &image_count,
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device
)
{
    Utils::Logs::log("Creating " + std::to_string(image_count) + " uniform buffers..");

    if (command_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Uniform buffers creation failed! The command pool provided (" + Utils::Text::get_memory_address(command_pool) + ") is not valid!");

    if (graphics_queue == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Uniform buffers creation failed! The graphics queue provided (" + Utils::Text::get_memory_address(graphics_queue) + ") is not valid!");

    if (image_count < 1)
        Utils::Logs::crash_error_log("Uniform buffers creation failed! The images count provided (" + std::to_string(image_count) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Uniform buffers creation failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Uniform buffers creation failed! The physical device provided (" + Utils::Text::get_memory_address(physical_device) + ") is not valid!");

    std::vector<UniformBufferInfo> output;
    output.reserve(image_count);
    const VkDeviceSize buffer_size = sizeof(UniformBufferObject);

    for (int i = 0; i < image_count; i++)
    {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory buffer_memory = VK_NULL_HANDLE;
        void* data;

        Vulkan::Buffers::create_buffer(buffer, buffer_memory, buffer_size, logical_device, physical_device, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        vkMapMemory(logical_device, buffer_memory, 0, buffer_size, 0, &data);

        const UniformBufferInfo info = { buffer, buffer_memory, data };
        output.emplace_back(info);

        Utils::Logs::log("- Uniform buffer #" + std::to_string(i + 1) + "/" + std::to_string(image_count) + " (" + Utils::Text::get_memory_address(buffer) + ") created successfully!");
    }

    Utils::Logs::log(std::to_string(output.size()) + " uniform buffers created successfully!");
    return output;
}



/*
    Update a uniform buffer.

    Tasks:
        1) 

    Parameters:
        - buffer_data / void*      / Data of the uniform buffer.
        - extent      / VkExtent2D / Vulkan swap chain resolution.
        - frame       / uint32_t   / Frame we are working on.

    Returns:
        No object returned.
*/
void Vulkan::Buffers::update_uniform_buffer_data
(
    const void* buffer_data,
    const VkExtent2D extent,
    const uint32_t &frame
)
{
    const static auto start_time = std::chrono::high_resolution_clock::now();

    const auto current_time = std::chrono::high_resolution_clock::now();
    const float time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();

    const glm::vec3 camera_position = glm::vec3(4.0f, 1.0f, 3.0f);
    const glm::vec3 camera_angle = glm::vec3(0.0f, 0.0f, 0.0f);

    UniformBufferObject object {};
    object.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    object.view = glm::lookAt(camera_position, camera_angle, glm::vec3(0.0f, 0.0f, 1.0f));
    object.projection = glm::perspective(glm::radians(45.0f), extent.width / (float) extent.height, 0.1f, 10.0f);
    object.projection[1][1] *= -1.0f;

    memcpy((void*) buffer_data, &object, sizeof(object));
}



/*
    Cleanly destroy uniform buffers.

    Tasks:
        1) Verify the parameters.
        2) Try to destroy each uniform buffer. We don't stop the iteration even if a buffer "failed" to destroy.

    Parameters:
        - logical_device  / VkDevice                  / Logical device of the Vulkan instance.
        - uniform_buffers / vector<UniformBufferInfo> / Uniform buffers to destroy.

    Returns:
        No object returned.
*/
void Vulkan::Buffers::destroy_uniform_buffers
(
    const VkDevice &logical_device,
    std::vector<UniformBufferInfo> &uniform_buffers
)
{
    Utils::Logs::log("Destroying " + std::to_string(uniform_buffers.size()) + " uniform buffers..");

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Uniform buffers destruction failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");
        return;
    }

    if (uniform_buffers.size() < 1)
    {
        Utils::Logs::error_log("Uniform buffers destruction failed! No uniform buffers provided!");
        return;
    }

    int failed = 0;
    int i = 0;

    for (UniformBufferInfo &uniform_buffer : uniform_buffers)
    {
        i++;

        VkBuffer buffer = uniform_buffer.buffer;
        VkDeviceMemory buffer_memory = uniform_buffer.buffer_memory;
        void* buffer_data = uniform_buffer.data;

        if (buffer == VK_NULL_HANDLE)
        {
            Utils::Logs::error_log("- Failed to destroy the uniform buffer #" + std::to_string(i) + "/" + std::to_string(uniform_buffers.size()) + "! The uniform buffer provided (" + Utils::Text::get_memory_address(buffer) + ") is not valid!");
            failed++;
            continue;
        }

        if (buffer_memory == VK_NULL_HANDLE)
        {
            Utils::Logs::error_log("- Failed to destroy the uniform buffer #" + std::to_string(i) + "/" + std::to_string(uniform_buffers.size()) + "! The uniform buffer memory provided (" + Utils::Text::get_memory_address(buffer_memory) + ") is not valid!");
            failed++;
            continue;
        }

        if (!buffer_data)
            Utils::Logs::error_log("- Failed to destroy the uniform buffer #" + std::to_string(i) + "/" + std::to_string(uniform_buffers.size()) + "! The uniform buffer data provided (" + Utils::Text::get_memory_address(buffer_data) + ") is not valid!");

        vkDestroyBuffer(logical_device, buffer, nullptr);
        vkFreeMemory(logical_device, buffer_memory, nullptr);

        buffer = VK_NULL_HANDLE;
        buffer_memory = VK_NULL_HANDLE;
        buffer_data = nullptr;

        Utils::Logs::log("- Uniform buffer #" + std::to_string(i) + "/" + std::to_string(uniform_buffers.size()) + " destroyed successfully!");
    }

    if (failed > 0)
        Utils::Logs::error_log("Warning: " + std::to_string(failed) + " uniform buffers failed to destroy! This might lead to some memory leaks or memory overload.");

    Utils::Logs::log(std::to_string(uniform_buffers.size() - failed) + "/" + std::to_string(uniform_buffers.size()) + " uniform buffers destroyed successfully!");
    uniform_buffers.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Vulkan::Buffers::uniform_buffers_handler::uniform_buffers_handler
(
    const VkCommandPool &command_pool,
    const VkQueue &graphics_queue,
    const uint32_t &image_count,
    const VkDevice &logical_device,
    const VkPhysicalDevice &physical_device
)
    : logical_device(logical_device)
{
    uniform_buffers = Vulkan::Buffers::create_uniform_buffers(command_pool, graphics_queue, image_count, logical_device, physical_device);
}

Vulkan::Buffers::uniform_buffers_handler::~uniform_buffers_handler()
{
    Vulkan::Buffers::destroy_uniform_buffers(logical_device, uniform_buffers);
}

std::vector<UniformBufferInfo> Vulkan::Buffers::uniform_buffers_handler::get() const
{
    return uniform_buffers;
}
