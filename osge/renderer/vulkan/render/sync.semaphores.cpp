#include "vulkan.render.hpp"

#include "../../../utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"

#include <cstdint>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a semaphore for each swap chain image.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Create a semaphore for each image.

    Parameters:
        - images_count   / uint32_t / Amount of swap chain images.
        - logical_device / VkDevice / Logical device of the Vulkan instance.

    Returns:
        A vector list containing all created semaphores.
*/
std::vector<VkSemaphore> Render::create_semaphores
(
    const uint32_t &images_count,
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Creating " + std::to_string(images_count) + " semaphores.. ", false);

    if (images_count < 1)
        Utils::Logs::crash_log("Failed! No swap chain images provided.");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    std::vector<VkSemaphore> semaphores;
    semaphores.reserve(images_count);

    for (int i = 0; i < images_count; i++)
    {
        /*
            - sType / Defines the type of the structure.
        */
        const VkSemaphoreCreateInfo create_info
        {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
        };

        VkSemaphore semaphore = VK_NULL_HANDLE;
        const VkResult semaphore_creation = vkCreateSemaphore(logical_device, &create_info, nullptr, &semaphore);

        if (semaphore_creation != VK_SUCCESS)
            Utils::Logs::crash_log("Failed! Creation #" + std::to_string(i + 1) + " returned error code -> " + std::to_string(semaphore_creation) + ".");

        semaphores.push_back(semaphore);
    }

    Utils::Logs::log("Done!", true);
    return semaphores;
}



/*
    Destroy some semaphores.

    Tasks:
        1) Verify function parameters.
        2) Destroy all valid semaphores.
        3) Set all objects to null.
        4) Empty vector list.

    Parameters:
        - logical_device / VkDevice            / Logical device of the Vulkan instance.
        - semaphores     / vector<VkSemaphore> / Semaphores to destroy.

    Returns:
        No object returned.
*/
void Render::destroy_semaphores
(
    const VkDevice           &logical_device,
    std::vector<VkSemaphore> &semaphores
)
{
    Utils::Logs::log("Destroying " + std::to_string(semaphores.size()) + " semaphores.. ", false);

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    if (semaphores.size() < 1)
    {
        Utils::Logs::log("Failed! No semaphores provided.", true);
        return;
    }

    for (VkSemaphore &semaphore : semaphores)
    {
        if (semaphore == VK_NULL_HANDLE)
            continue;

        vkDestroySemaphore(logical_device, semaphore, nullptr);
        semaphore = VK_NULL_HANDLE;
    }

    Utils::Logs::log("Done!", true);
    semaphores.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Render::sync_semaphores_handler::sync_semaphores_handler
(
    const uint32_t &images_count,
    const VkDevice &logical_device
)
    : logical_device(logical_device)
{
    semaphores = create_semaphores(images_count, logical_device);
}

Render::sync_semaphores_handler::~sync_semaphores_handler()
{
    destroy_semaphores(logical_device, semaphores);
}

std::vector<VkSemaphore> Render::sync_semaphores_handler::get() const
{
    return semaphores;
}
