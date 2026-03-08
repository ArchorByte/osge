#include "vulkan.queues.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <string>
#include <unistd.h>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a command pool.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Create command pool.

    Parameters:
        - graphics_family_index / uint32_t / Index of the graphics queue family.
        - logical_device        / VkDevice / Logical device of this Vulkan instance.

    Returns:
        The created command pool.
*/
VkCommandPool Queues::create_command_pool
(
    const uint32_t &graphics_family_index,
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Creating command pool.. ", false);

    if (graphics_family_index < 0)
        Utils::Logs::crash_log("Failed! Graphics family index invalid -> " + std::to_string(graphics_family_index) + ".");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    /*
        - sType            / Defines the type of the structure.
        - flags            / Defines to Vulkan what we are going to do with this command pool.
        - queueFamilyIndex / Defines the index of the graphics queue family.
    */
    const VkCommandPoolCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = graphics_family_index
    };

    VkCommandPool command_pool = VK_NULL_HANDLE;
    const VkResult pool_creation = vkCreateCommandPool(logical_device, &create_info, nullptr, &command_pool);

    if (pool_creation != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Creation returned error code -> " + std::to_string(pool_creation) + ".");

    Utils::Logs::log("Done! Memory address -> " + Utils::Text::get_memory_address(command_pool) + ".", true);
    return command_pool;
}



/*
    Destroy a command pool.

    Tasks:
        1) Verify function parameters.
        2) Destroy the command pool.
        3) Set object to null.

    Parameters:
        - command_pool   / VkCommandPool / Command pool to destroy.
        - logical_device / VkDevice      / Logical device of this Vulkan instance.

    Returns:
        No object returned.
*/
void Queues::destroy_command_pool
(
    VkCommandPool  &command_pool,
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Destroying command pool (" + Utils::Text::get_memory_address(command_pool) + ").. ", false);

    if (command_pool == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Command pool invalid.", true);
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    vkDestroyCommandPool(logical_device, command_pool, nullptr);
    command_pool = VK_NULL_HANDLE;

    Utils::Logs::log("Done!", true);
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Queues::command_pool_handler::command_pool_handler
(
    const uint32_t &graphics_family_index,
    const VkDevice &logical_device
)
    : logical_device(logical_device)
{
    command_pool = create_command_pool(graphics_family_index, logical_device);
}

Queues::command_pool_handler::~command_pool_handler()
{
    destroy_command_pool(command_pool, logical_device);
}

VkCommandPool Queues::command_pool_handler::get() const
{
    return command_pool;
}
