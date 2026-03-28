#include "vulkan.buffers.hpp"

#include "libraries/glm/glm.hpp"
#include "libraries/glm/gtc/matrix_transform.hpp"
#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <chrono>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create the uniform buffers.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Create a uniform buffer for each swap chain image.
        3) Get the created buffer information.

    Parameters:
        - command_pool    / VkCommandPool    / Handles memory allocation of command buffers.
        - graphics_queue  / VkQueue          / Handles all graphics commands and calls.
        - image_count     / uint32_t         / Amount of uniform buffers to create.
        - logical_device  / VkDevice         / Logical device of the Vulkan instance.
        - physical_device / VkPhysicalDevice / Physical device used to run this Vulkan instance.

    Returns:
        A vector list containing all created uniform buffers.
*/
std::vector<UniformBufferInfo> Buffers::create_uniform_buffers
(
    const VkCommandPool    &command_pool,
    const VkQueue          &graphics_queue,
    const uint32_t         &image_count,
    const VkDevice         &logical_device,
    const VkPhysicalDevice &physical_device
)
{
    Utils::Logs::log("Creating " + std::to_string(image_count) + " uniform buffers.. ", false);

    if (command_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Command pool invalid!");

    if (graphics_queue == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Graphics queue invalid!");

    if (image_count < 1)
        Utils::Logs::crash_log("Failed! Images count invalid -> " + std::to_string(image_count) + ".");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical device invalid.");

    std::vector<UniformBufferInfo> output;
    output.reserve(image_count);
    const VkDeviceSize buffer_size = sizeof(UniformBufferObject);

    for (int i = 0; i < image_count; i++)
    {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory buffer_memory = VK_NULL_HANDLE;
        void* data;

        Buffers::create_buffer(buffer, buffer_memory, buffer_size, logical_device, physical_device, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        vkMapMemory(logical_device, buffer_memory, 0, buffer_size, 0, &data); // Retrieve the buffer data.

        const UniformBufferInfo info = { buffer, buffer_memory, data };
        output.emplace_back(info);
    }

    Utils::Logs::log("Done!", true);
    return output;
}



/*
    Update a uniform buffer.

    Parameters:
        - buffer_data / void*      / Data of the uniform buffer.
        - extent      / VkExtent2D / Resolution of the swap chain.
        - frame       / uint32_t   / Frame we are working on.

    Returns:
        No object returned.
*/
void Buffers::update_uniform_buffer_data
(
    const void*      buffer_data,
    const VkExtent2D &extent,
    const uint32_t   &frame
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
    Destroy some uniform buffers.

    Tasks:
        1) Verify function parameters.
        2) Destroy all valid uniform buffers and free their allocated memory.
        3) Set all objects to null.
        4) Empty vector list.

    Parameters:
        - logical_device  / VkDevice                  / Logical device of the Vulkan instance.
        - uniform_buffers / vector<UniformBufferInfo> / Uniform buffers to destroy.

    Returns:
        No object returned.
*/
void Buffers::destroy_uniform_buffers
(
    const VkDevice                 &logical_device,
    std::vector<UniformBufferInfo> &uniform_buffers
)
{
    Utils::Logs::log("Destroying " + std::to_string(uniform_buffers.size()) + " uniform buffers.. ", false);

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    if (uniform_buffers.size() < 1)
    {
        Utils::Logs::log("Done!", true);
        return;
    }

    for (UniformBufferInfo &uniform_buffer : uniform_buffers)
    {
        VkBuffer buffer = uniform_buffer.buffer;
        VkDeviceMemory buffer_memory = uniform_buffer.buffer_memory;
        void* buffer_data = uniform_buffer.data;

        if (buffer == VK_NULL_HANDLE)
            continue;

        if (buffer_memory == VK_NULL_HANDLE)
            continue;

        vkDestroyBuffer(logical_device, buffer, nullptr);
        vkFreeMemory(logical_device, buffer_memory, nullptr);

        buffer = VK_NULL_HANDLE;
        buffer_memory = VK_NULL_HANDLE;
        buffer_data = nullptr;
    }

    Utils::Logs::log("Done!", true);
    uniform_buffers.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Buffers::uniform_buffers_handler::uniform_buffers_handler
(
    const VkCommandPool    &command_pool,
    const VkQueue          &graphics_queue,
    const uint32_t         &image_count,
    const VkDevice         &logical_device,
    const VkPhysicalDevice &physical_device
)
    : logical_device(logical_device)
{
    uniform_buffers = create_uniform_buffers(command_pool, graphics_queue, image_count, logical_device, physical_device);
}

Buffers::uniform_buffers_handler::~uniform_buffers_handler()
{
    destroy_uniform_buffers(logical_device, uniform_buffers);
}

std::vector<UniformBufferInfo> Buffers::uniform_buffers_handler::get() const
{
    return uniform_buffers;
}
