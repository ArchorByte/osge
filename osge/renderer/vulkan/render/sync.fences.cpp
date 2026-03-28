#include "vulkan.render.hpp"

#include "../../../utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"

#include <cstdint>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a fence for each swap chain image.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Create a fence for each image.

    Parameters:
        - images_count   / uint32_t / Amount of swap chain images.
        - logical_device / VkDevice / Logical device of the Vulkan instance.

    Returns:
        A vector list containing all created fences.
*/
std::vector<VkFence> Render::create_fences
(
    const uint32_t &images_count,
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Creating " + std::to_string(images_count) + " fences.. ", false);

    if (images_count < 1)
        Utils::Logs::crash_log("Failed! No swap chain images provided.");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    std::vector<VkFence> fences;
    fences.reserve(images_count);

    for (int i = 0; i < images_count; i++)
    {
        /*
            - sType / Defines the type of the structure.
            - flags / Defines the initial state and behavior of the fence.
        */
        const VkFenceCreateInfo create_info
        {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT
        };

        VkFence fence = VK_NULL_HANDLE;
        const VkResult fence_creation = vkCreateFence(logical_device, &create_info, nullptr, &fence);

        if (fence_creation != VK_SUCCESS)
            Utils::Logs::crash_log("Failed! Creation #" + std::to_string(i + 1) + " returned error code -> " + std::to_string(fence_creation) + ".");

        fences.emplace_back(fence);
    }

    Utils::Logs::log("Done!", true);
    return fences;
}



/*
    Destroy some fences.

    Tasks:
        1) Verify function parameters.
        2) Destroy all valid fences.
        3) Set all objects to null.
        4) Empty vector list.

    Parameters:
        - fences         / vector<VkFence> / Fences to destroy.
        - logical_device / VkDevice        / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void destroy_fences
(
    std::vector<VkFence> &fences,
    const VkDevice       &logical_device
)
{
    Utils::Logs::log("Destroying " + std::to_string(fences.size()) + " fences.. ", false);

    if (fences.size() < 1)
    {
        Utils::Logs::log("Failed! No fences provided.", true);
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    for (VkFence fence : fences)
    {
        if (fence == VK_NULL_HANDLE)
            continue;

        vkDestroyFence(logical_device, fence, nullptr);
        fence = VK_NULL_HANDLE;
    }

    Utils::Logs::log("Done!", true);
    fences.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Render::sync_fences_handler::sync_fences_handler
(
    const uint32_t &images_count,
    const VkDevice &logical_device
)
    : logical_device(logical_device)
{
    fences = create_fences(images_count, logical_device);
}

Render::sync_fences_handler::~sync_fences_handler()
{
    destroy_fences(fences, logical_device);
}

std::vector<VkFence> Render::sync_fences_handler::get() const
{
    return fences;
}
