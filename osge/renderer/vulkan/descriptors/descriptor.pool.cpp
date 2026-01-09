#include "vulkan.descriptors.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a descriptor pool.
    Note: You should use the pre-made class to handle the descriptor pool rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the parameters.
        2) Make the pool sizes.
        3) Create the descriptor pool.

    Parameters:
        - image_count         / uint32_t / Amount of descriptors to make.
        - logical_device      / VkDevice / Logical device of the Vulkan instance.
        - texture_image_count / uint32_t / Amount of texture images to work with.

    Returns:
        The created descriptor pool.
*/
VkDescriptorPool Vulkan::Descriptors::create_descriptor_pool
(
    const uint32_t &image_count,
    const VkDevice &logical_device,
    const uint32_t &texture_image_count
)
{
    log("Creating a descriptor pool..");

    if (image_count < 1)
        fatal_error_log("Descriptor pool creation failed! The images count provided (" + std::to_string(image_count) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        fatal_error_log("Descriptor pool creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");

    if (texture_image_count < 1)
        fatal_error_log("Descriptor pool creation failed! The texture images count provided (" + std::to_string(texture_image_count) + ") is not valid!");

    std::vector<VkDescriptorPoolSize> pool_sizes(2);
    pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool_sizes[0].descriptorCount = static_cast<uint32_t>(image_count);
    pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    pool_sizes[1].descriptorCount = static_cast<uint32_t>(image_count * texture_image_count);

    const VkDescriptorPoolCreateInfo creation_info
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = static_cast<uint32_t>(image_count),
        .poolSizeCount = static_cast<uint32_t>(pool_sizes.size()),
        .pPoolSizes = pool_sizes.data()
    };

    VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
    const VkResult pool_creation = vkCreateDescriptorPool(logical_device, &creation_info, nullptr, &descriptor_pool);

    if (pool_creation != VK_SUCCESS)
        fatal_error_log("Descriptor pool creation returned error code " + std::to_string(pool_creation) + ".");

    log("Descriptor pool " + force_string(descriptor_pool) + " created successfully!");
    return descriptor_pool;
}



/*
    Cleanly destroy a descriptor pool.

    Tasks:
        1) Verify the parameters.
        2) Destroy the descriptor pool.
        3) Replace the objects addressess.

    Parameters:
        - descriptor_pool / VkDescriptorPool / Descriptor pool to destroy.
        - logical_device  / VkDevice         / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Vulkan::Descriptors::destroy_descriptor_pool
(
    VkDescriptorPool &descriptor_pool,
    const VkDevice &logical_device
)
{
    log("Destroying the " + force_string(descriptor_pool) + " descriptor pool..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Descriptor pool destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (descriptor_pool == VK_NULL_HANDLE)
    {
        error_log("Descriptor pool destruction failed! The descriptor pool provided (" + force_string(descriptor_pool) + ") is not valid!");
        return;
    }

    vkDestroyDescriptorPool(logical_device, descriptor_pool, nullptr);
    descriptor_pool = VK_NULL_HANDLE;

    log("Descriptor pool destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Vulkan::Descriptors::descriptor_pool_handler::descriptor_pool_handler
(
    const uint32_t &image_count,
    const VkDevice &logical_device,
    const uint32_t &texture_image_count
)
    : logical_device(logical_device)
{
    descriptor_pool = Vulkan::Descriptors::create_descriptor_pool(image_count, logical_device, texture_image_count);
}

Vulkan::Descriptors::descriptor_pool_handler::~descriptor_pool_handler()
{
    Vulkan::Descriptors::destroy_descriptor_pool(descriptor_pool, logical_device);
}

VkDescriptorPool Vulkan::Descriptors::descriptor_pool_handler::get() const
{
    return descriptor_pool;
}
