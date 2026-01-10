#include "vulkan.queues.hpp"
#include "osge/utils/utils.hpp"
#include <libraries/vulkan/vulkan.h>
#include <string>
#include <unistd.h>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a command pool.
    Note: You should use the pre-made class to handle the command pool rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the parameters.
        2) Create the command pool.

    Parameters:
        - graphics_family_index / uint32_t / Index of the graphics queue family.
        - logical_device        / VkDevice / Logical device of the Vulkan instance.

    Returns:
        The created command pool.
*/
VkCommandPool Vulkan::Queues::create_command_pool
(
    const uint32_t &graphics_family_index,
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Creating a command pool..");

    if (graphics_family_index < 0)
        Utils::Logs::crash_error_log("Command pool creation failed! The graphics family index provided (" + std::to_string(graphics_family_index) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Command pool creation failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    const VkCommandPoolCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = graphics_family_index
    };

    VkCommandPool command_pool = VK_NULL_HANDLE;
    const VkResult pool_creation = vkCreateCommandPool(logical_device, &create_info, nullptr, &command_pool);

    if (pool_creation != VK_SUCCESS)
        Utils::Logs::crash_error_log("Command pool creation returned error code " + std::to_string(pool_creation) + ".");

    Utils::Logs::log("Command pool " + Utils::Text::get_memory_address(command_pool) + " created successfully!");
    return command_pool;
}



/*
    Cleanly destroy a command pool.

    Tasks:
        1) Verify the parameters.
        2) Destroy the command pool.
        3) Get rid of the object address memory.

    Parameters:
        - command_pool   / VkCommandPool / Command pool to destroy.
        - logical_device / VkDevice      / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Vulkan::Queues::destroy_command_pool
(
    VkCommandPool &command_pool,
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Destroying the " + Utils::Text::get_memory_address(command_pool) + " command pool..");

    if (command_pool == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Command pool destruction failed! The command pool provided (" + Utils::Text::get_memory_address(command_pool) + ") is not valid!");
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Command pool destruction failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");
        return;
    }

    vkDestroyCommandPool(logical_device, command_pool, nullptr);
    command_pool = VK_NULL_HANDLE;

    Utils::Logs::log("Command pool destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Vulkan::Queues::command_pool_handler::command_pool_handler
(
    const uint32_t &graphics_family_index,
    const VkDevice &logical_device
) : logical_device(logical_device)
{
    command_pool = Vulkan::Queues::create_command_pool(graphics_family_index, logical_device);
}

Vulkan::Queues::command_pool_handler::~command_pool_handler()
{
    Vulkan::Queues::destroy_command_pool(command_pool, logical_device);
}

VkCommandPool Vulkan::Queues::command_pool_handler::get() const
{
    return command_pool;
}
