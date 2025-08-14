#include "pool.hpp"

#include "../../../logs/terminal.hpp"
#include "../../../tools/text_format.hpp"

#include <vulkan/vulkan.h>
#include <cstdint>
#include <string>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a descriptor pool.
VkDescriptorPool create_vulkan_descriptor_pool
(
    const VkDevice &logical_device,
    const uint32_t &images_count
)
{
    log("Creating a descriptor pool..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Descriptor pool creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (images_count < 1)
    {
        fatal_error_log("Descriptor pool creation failed! The images count provided (" + std::to_string(images_count) + ") is not valid!");
    }

    // Size of the pool descriptor.
    VkDescriptorPoolSize pool_size {};
    pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool_size.descriptorCount = static_cast<uint32_t>(images_count); // Amount of descriptors to create.

    // Create info for the descriptor pool.
    VkDescriptorPoolCreateInfo pool_info {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.poolSizeCount = 1;                             // Amount of pool sizes to pass.
    pool_info.pPoolSizes = &pool_size;                       // Pass the pool size.
    pool_info.maxSets = static_cast<uint32_t>(images_count); // Maxomum amount of sets.

    VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
    VkResult pool_creation = vkCreateDescriptorPool(logical_device, &pool_info, nullptr, &descriptor_pool); // Try to create the pool.

    if (pool_creation != VK_SUCCESS)
    {
        fatal_error_log("Descriptor pool creation returned error code " + std::to_string(pool_creation) + ".");
    }

    if (!descriptor_pool || descriptor_pool == VK_NULL_HANDLE)
    {
        fatal_error_log("Descriptor pool creation output \"" + force_string(descriptor_pool) + "\" is not valid!");
    }

    log("Descriptor pool " + force_string(descriptor_pool) + " created successfully!");
    return descriptor_pool;
}

// Destroy a descriptor pool.
void destroy_vulkan_descriptor_pool
(
    const VkDevice &logical_device,
    VkDescriptorPool &descriptor_pool
)
{
    log("Destroying the " + force_string(descriptor_pool) + " descriptor pool..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        error_log("Descriptor pool destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (!descriptor_pool || descriptor_pool == VK_NULL_HANDLE)
    {
        error_log("Descriptor pool destruction failed! The descriptor pool provided (" + force_string(descriptor_pool) + ") is not valid!");
        return;
    }

    // Destroy the descriptor pool and dispose of the address.
    vkDestroyDescriptorPool(logical_device, descriptor_pool, nullptr);
    descriptor_pool = VK_NULL_HANDLE;

    log("Descriptor pool destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_DescriptorPool::Vulkan_DescriptorPool
(
    const VkDevice &logical_device,
    const uint32_t &images_count
) : logical_device(logical_device)
{
    descriptor_pool = create_vulkan_descriptor_pool(logical_device, images_count);
}

// Destructor.
Vulkan_DescriptorPool::~Vulkan_DescriptorPool()
{
    destroy_vulkan_descriptor_pool(logical_device, descriptor_pool);
}

VkDescriptorPool Vulkan_DescriptorPool::get() const
{
    return descriptor_pool;
}
