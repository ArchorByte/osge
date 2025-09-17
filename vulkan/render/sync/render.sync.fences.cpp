#include "render.sync.fences.hpp"

#include "../../../logs/logs.handler.hpp"
#include "../../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a fence for each swap chain image.
std::vector<VkFence> create_vulkan_fences
(
    const VkDevice &logical_device,
    const uint32_t images_count
)
{
    log("Creating " + std::to_string(images_count) + " fences..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Fences creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (images_count < 1 || !images_count)
    {
        fatal_error_log("Fences creation failed! No swap chain images were provided!");
    }

    std::vector<VkFence> fences;

    // Create a fence for each swap chain image.
    for (int i = 0; i < images_count; i++)
    {
        // Create info for the fence.
        VkFenceCreateInfo info {};
        info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        info.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Some flags from the docs.

        VkFence fence = VK_NULL_HANDLE;
        VkResult fence_creation = vkCreateFence(logical_device, &info, nullptr, &fence); // Trying to create the fence.

        if (fence_creation != VK_SUCCESS)
        {
            fatal_error_log("Fence #" + std::to_string(i + 1) + "/" + std::to_string(images_count) + " creation returned error code " + std::to_string(fence_creation) + ".");
        }

        if (!fence || fence == VK_NULL_HANDLE)
        {
            fatal_error_log("Fence #" + std::to_string(i + 1) + "/" + std::to_string(images_count) + " creation output \"" + force_string(fence) + "\" is not valid.");
        }

        fences.emplace_back(fence); // Register the fence.
        log("- Fence #" + std::to_string(i + 1) + "/" + std::to_string(images_count) + " (" + force_string(fence) + ") created successfully!");
    }

    log(std::to_string(images_count) + " fences created successfully!");
    return fences;
}

// Destroy some Vulkan fences.
void destroy_vulkan_fences
(
    const VkDevice &logical_device,
    std::vector<VkFence> &fences
)
{
    log("Destroying " + std::to_string(fences.size()) + " fences..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        error_log("Fences destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (fences.size() < 1)
    {
        error_log("Fences destruction failed! No fences were provided!");
        return;
    }

    int failed = 0;
    int i = 0;

    // Destroy each fence in the list.
    for (VkFence fence : fences)
    {
        i++;

        if (!fence || fence == VK_NULL_HANDLE)
        {
            error_log("- Warning: Failed to destroy the fence #" + std::to_string(i) + "/" + std::to_string(fences.size()) + "! The fence provided (" + force_string(fence) + ") is not valid!");
            failed++;
            continue;
        }

        // Destroy the fence and dispose of the address.
        vkDestroyFence(logical_device, fence, nullptr);
        fence = VK_NULL_HANDLE;

        log("- Fence #" + std::to_string(i) + "/" + std::to_string(fences.size()) + " destroyed successfully!");
    }

    if (failed > 0) error_log("Warning: " + std::to_string(failed) + " fences failed to destroy! This might lead to some memory leaks and memory overload.");
    log(std::to_string(fences.size() - failed) + "/" + std::to_string(fences.size()) + " fences destroyed successfully!");

    // Free the list.
    fences.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_Fence::Vulkan_Fence
(
    const VkDevice &logical_device,
    const uint32_t images_count
) : logical_device(logical_device)
{
    fences = create_vulkan_fences(logical_device, images_count);
}

// Destructor.
Vulkan_Fence::~Vulkan_Fence()
{
    destroy_vulkan_fences(logical_device, fences);
}

std::vector<VkFence> Vulkan_Fence::get() const
{
    return fences;
}
