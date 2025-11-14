#include "descriptor.pool.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <cstdint>
#include <string>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a descriptor pool.
VkDescriptorPool create_vulkan_descriptor_pool
(
    const VkDevice &logical_device,
    const uint32_t &images_count,
    const uint32_t &texture_images_count
)
{
    log("Creating a descriptor pool..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Descriptor pool creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (images_count < 1)
    {
        fatal_error_log("Descriptor pool creation failed! The images count provided (" + std::to_string(images_count) + ") is not valid!");
    }

    if (texture_images_count < 1)
    {
        fatal_error_log("Descriptor pool creation failed! The texture images count provided (" + std::to_string(texture_images_count) + ") is not valid!");
    }

    std::vector<VkDescriptorPoolSize> pool_sizes(2);
    pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;              // Descriptor pool for uniform buffers.
    pool_sizes[0].descriptorCount = static_cast<uint32_t>(images_count); // Amount of descriptors to create.
    pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;      // Pool for an image sampler.
    pool_sizes[1].descriptorCount = static_cast<uint32_t>(images_count * texture_images_count);

    const VkDescriptorPoolCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = static_cast<uint32_t>(images_count),            // Maximum amount of sets to make.
        .poolSizeCount = static_cast<uint32_t>(pool_sizes.size()), // Amount of pool sizes to pass.
        .pPoolSizes = pool_sizes.data()                            // Pass the pool sizes.
    };

    VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
    const VkResult pool_creation = vkCreateDescriptorPool(logical_device, &create_info, nullptr, &descriptor_pool);

    if (pool_creation != VK_SUCCESS)
    {
        fatal_error_log("Descriptor pool creation returned error code " + std::to_string(pool_creation) + ".");
    }

    if (descriptor_pool == VK_NULL_HANDLE)
    {
        fatal_error_log("Descriptor pool creation output (" + force_string(descriptor_pool) + ") is not valid!");
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

// Constructor.
Vulkan_DescriptorPool::Vulkan_DescriptorPool
(
    const VkDevice &logical_device,
    const uint32_t &images_count,
    const uint32_t &texture_images_count
) : logical_device(logical_device)
{
    descriptor_pool = create_vulkan_descriptor_pool(logical_device, images_count, texture_images_count);
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
