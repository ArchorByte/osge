#include "render.sync.semaphores.hpp"

#include "../../../logs/logs.handler.hpp"
#include "../../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a semaphore for each swap chain image.
std::vector<VkSemaphore> create_vulkan_semaphores
(
    const VkDevice &logical_device,
    const uint32_t &images_count
)
{
    log("Creating " + std::to_string(images_count) + " semaphores..");
    std::vector<VkSemaphore> semaphores;

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Semaphores creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (images_count < 1 || !images_count)
    {
        fatal_error_log("Semaphores creation failed! No swap chain images were provided!");
    }

    // We create a semaphore for each image.
    for (int i = 0; i < images_count; i++)
    {
        // Create info for the semaphores.
        VkSemaphoreCreateInfo info {};
        info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkSemaphore semaphore = VK_NULL_HANDLE;
        VkResult semaphore_creation = vkCreateSemaphore(logical_device, &info, nullptr, &semaphore); // Try to create the semaphore.

        if (semaphore_creation != VK_SUCCESS)
        {
            fatal_error_log("Semaphore #" + std::to_string(i + 1) + "/" + std::to_string(images_count) + " creation returned error code " + std::to_string(semaphore_creation) + ".");
        }

        if (!semaphore || semaphore == VK_NULL_HANDLE)
        {
            fatal_error_log("Semaphore #" + std::to_string(i + 1) + "/" + std::to_string(images_count) + " creation output \"" + force_string(semaphore) + "\" is not valid!");
        }

        semaphores.push_back(semaphore); // Register the semaphore.
        log("- Semaphore #" + std::to_string(i + 1) + "/" + std::to_string(images_count) + " (" + force_string(semaphore) + ") created successfully!");
    }

    log(std::to_string(images_count) + " semaphores created successfully!");
    return semaphores;
}

// Destroy some Vulkan semaphores.
void destroy_semaphores
(
    const VkDevice &logical_device,
    std::vector<VkSemaphore> &semaphores
)
{
    log("Destroying " + std::to_string(semaphores.size()) + " semaphores..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        error_log("Semaphores destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (semaphores.size() < 1)
    {
        error_log("Semaphores destruction failed! No semaphores were provided!");
        return;
    }

    int failed = 0;
    int i = 0;

    // Destroy each semaphore in the list.
    for (VkSemaphore &semaphore : semaphores)
    {
        i++;

        if (!semaphore || semaphore == VK_NULL_HANDLE)
        {
            error_log("- Warning: Failed to destroy the semaphore #" + std::to_string(i) + "/" + std::to_string(semaphores.size()) + "! The semaphore provided (" + force_string(semaphore) + ") is not valid!");
            failed++;
            continue;
        }

        // Destroy the semaphore and dispose of the address.
        vkDestroySemaphore(logical_device, semaphore, nullptr);
        semaphore = VK_NULL_HANDLE;

        log("- Semaphore #" + std::to_string(i) + "/" + std::to_string(semaphores.size()) + " destroyed successfully!");
    }

    if (failed > 0) error_log("Warning: " + std::to_string(failed) + " semaphores failed to destroy! This might leads to some memory leaks and memory overload!");
    log(std::to_string(semaphores.size() - failed) + "/" + std::to_string(semaphores.size()) + " semaphores destroyed successfully!");

    // Free the list.
    semaphores.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_Semaphores::Vulkan_Semaphores
(
    const VkDevice &logical_device,
    const uint32_t &images_count
) : logical_device(logical_device)
{
    semaphores = create_vulkan_semaphores(logical_device, images_count);
}

// Destructor.
Vulkan_Semaphores::~Vulkan_Semaphores()
{
    destroy_semaphores(logical_device, semaphores);
}

std::vector<VkSemaphore> Vulkan_Semaphores::get() const
{
    return semaphores;
}
