#include "vulkan.descriptors.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <string>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a descriptor pool.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Make pool sizes.
        3) Create the descriptor pool.

    Parameters:
        - image_count         / uint32_t / Defines the maximum amount of descriptor sets that we can create with this pool.
        - logical_device      / VkDevice / Logical device of the Vulkan instance.
        - texture_image_count / uint32_t / Amount of texture images to work with.

    Returns:
        The created descriptor pool.
*/
VkDescriptorPool Descriptors::create_descriptor_pool
(
    const uint32_t &image_count,
    const VkDevice &logical_device,
    const uint32_t &texture_image_count
)
{
    Utils::Logs::log("Creating descriptor pool.. ", false);

    if (image_count < 1)
        Utils::Logs::crash_log("Failed! Images count invalid -> " + std::to_string(image_count) + ".");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (texture_image_count < 1)
        Utils::Logs::crash_log("Failed! Texture images count invalid -> " + std::to_string(texture_image_count) + ".");

    /*
        - type            / Defines the type of the structure.
        - descriptorCount / Defines the maximum amount of descriptors that we can handle.
    */
    std::vector<VkDescriptorPoolSize> pool_sizes(2);
    pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool_sizes[0].descriptorCount = static_cast<uint32_t>(image_count);
    pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    pool_sizes[1].descriptorCount = static_cast<uint32_t>(image_count * texture_image_count);

    /*
        - sType         / Defines the type of the structure.
        - maxSets       / Defines the maximum amount of descriptor sets that we can handle.
        - poolSizeCount / Defines the amount of pool sizes to pass.
        - pPoolSizes    / Passes the pool sizes.
    */
    const VkDescriptorPoolCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = static_cast<uint32_t>(image_count),
        .poolSizeCount = static_cast<uint32_t>(pool_sizes.size()),
        .pPoolSizes = pool_sizes.data()
    };

    VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
    const VkResult pool_creation = vkCreateDescriptorPool(logical_device, &create_info, nullptr, &descriptor_pool);

    if (pool_creation != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Creation returned error code -> " + std::to_string(pool_creation) + ".");

    Utils::Logs::log("Done! Memory address -> " + Utils::Text::get_memory_address(descriptor_pool) + ".", true);
    return descriptor_pool;
}



/*
    Destroy a descriptor pool.

    Tasks:
        1) Verify function parameters.
        2) Destroy the descriptor pool.
        3) Set object to null.

    Parameters:
        - descriptor_pool / VkDescriptorPool / Descriptor pool to destroy.
        - logical_device  / VkDevice         / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Descriptors::destroy_descriptor_pool
(
    VkDescriptorPool &descriptor_pool,
    const VkDevice   &logical_device
)
{
    Utils::Logs::log("Destroying descriptor pool (" + Utils::Text::get_memory_address(descriptor_pool) + ").. ", false);

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    if (descriptor_pool == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Descriptor pool invalid.", true);
        return;
    }

    vkDestroyDescriptorPool(logical_device, descriptor_pool, nullptr);
    descriptor_pool = VK_NULL_HANDLE;

    Utils::Logs::log("Done!", true);
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Descriptors::descriptor_pool_handler::descriptor_pool_handler
(
    const uint32_t &image_count,
    const VkDevice &logical_device,
    const uint32_t &texture_image_count
)
    : logical_device(logical_device)
{
    descriptor_pool = create_descriptor_pool(image_count, logical_device, texture_image_count);
}

Descriptors::descriptor_pool_handler::~descriptor_pool_handler()
{
    destroy_descriptor_pool(descriptor_pool, logical_device);
}

VkDescriptorPool Descriptors::descriptor_pool_handler::get() const
{
    return descriptor_pool;
}
