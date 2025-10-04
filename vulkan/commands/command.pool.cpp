#include "command.pool.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <unistd.h>
#include <cstdint>
#include <string>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a command pool.
VkCommandPool create_vulkan_command_pool
(
    const VkDevice &logical_device,
    const uint32_t &graphics_family_index
)
{
    log("Creating a command pool..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Command pool creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (graphics_family_index < 0)
    {
        fatal_error_log("Command pool creation failed! The graphics family index provided (" + std::to_string(graphics_family_index) + ") is not valid!");
    }

    VkCommandPoolCreateInfo creation_info {};
    creation_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    creation_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Allow the command buffers to reset.
    creation_info.queueFamilyIndex = graphics_family_index;                // Pass the index of the graphics family.

    VkCommandPool command_pool = VK_NULL_HANDLE;
    const VkResult pool_creation = vkCreateCommandPool(logical_device, &creation_info, nullptr, &command_pool);

    if (pool_creation != VK_SUCCESS)
    {
        fatal_error_log("Command pool creation returned error code " + std::to_string(pool_creation) + ".");
    }

    if (command_pool == VK_NULL_HANDLE)
    {
        fatal_error_log("Command pool creation output (" + force_string(command_pool) + ") is not valid!");
    }

    log("Command pool " + force_string(command_pool) + " created successfully!");
    return command_pool;
}

// Destroy a command pool.
void destroy_vulkan_command_pool
(
    const VkDevice &logical_device,
    VkCommandPool &command_pool
)
{
    log("Destroying the " + force_string(command_pool) + " command pool..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Command pool destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (command_pool == VK_NULL_HANDLE)
    {
        error_log("Command pool destruction failed! The command pool provided (" + force_string(command_pool) + ") is not valid!");
        return;
    }

    vkDestroyCommandPool(logical_device, command_pool, nullptr);
    command_pool = VK_NULL_HANDLE;

    log("Command pool destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_CommandPool::Vulkan_CommandPool
(
    const VkDevice &logical_device,
    const uint32_t &graphics_family_index
) : logical_device(logical_device)
{
    command_pool = create_vulkan_command_pool(logical_device, graphics_family_index);
}

// Destructor.
Vulkan_CommandPool::~Vulkan_CommandPool()
{
    destroy_vulkan_command_pool(logical_device, command_pool);
}

VkCommandPool Vulkan_CommandPool::get() const
{
    return command_pool;
}
