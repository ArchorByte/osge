#include "vulkan.descriptors.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/renderer/vulkan/buffers/vulkan.buffers.hpp"
#include "osge/utils/utils.hpp"

#include <string>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a descriptor set for each swap chain image.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Allocate some memory to all descriptor sets.
        3) Make two write sets. One for the uniform buffers, and the other for all texture images.
        4) Update all descriptor sets data.

    Parameters:
        - descriptor_set_layout / VkDescriptorSetLayout     / Describes the structure of a descriptor set.
        - descriptor_pool       / VkDescriptorPool          / Handles memory allocation of descriptor sets.
        - image_count           / uint32_t                  / Defines the amount of descriptor sets to create.
        - logical_device        / VkDevice                  / Logical device of this Vulkan instance.
        - texture_image_views   / vector<VkImageView>       / Describes how to access and treat the texture images.
        - texture_sampler       / VkSampler                 / Enables us to read image data and to apply effects for shaders.
        - uniform_buffers       / vector<UniformBufferInfo> / Enables us to transfer some data to shaders.

    Returns:
        A vector list containing all created descriptor sets.
*/
std::vector<VkDescriptorSet> Descriptors::create_descriptor_sets
(
    const VkDescriptorSetLayout          &descriptor_set_layout,
    const VkDescriptorPool               &descriptor_pool,
    const uint32_t                       &image_count,
    const VkDevice                       &logical_device,
    const std::vector<VkImageView>       &texture_image_views,
    const VkSampler                      &texture_sampler,
    const std::vector<UniformBufferInfo> &uniform_buffers
)
{
    Utils::Logs::log("Creating " + std::to_string(image_count) + " descriptor sets.. ", false);

    if (descriptor_set_layout == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Descriptor set layout invalid.");

    if (descriptor_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Descriptor pool invalid.");

    if (image_count < 1)
        Utils::Logs::crash_log("Failed! Images count invalid -> " + std::to_string(image_count) + ".");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (texture_image_views.size() < 1)
        Utils::Logs::crash_log("Failed! No texture image views provided.");

    if (texture_sampler == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Texture sampler invalid.");

    if (uniform_buffers.size() < 1)
        Utils::Logs::crash_log("Failed! No uniform buffers provided.");

    std::vector<VkDescriptorSet> descriptor_sets(image_count);
    std::vector<VkDescriptorSetLayout> layouts(image_count, descriptor_set_layout);

    /*
        - sType              / Defines the type of the structure.
        - descriptorPool     / Defines which descriptor pool to use for the memory allocation.
        - descriptorSetCount / Amount of descriptor sets to make.
        - pSetLayouts        / Passes all descriptor set layouts.
    */
    VkDescriptorSetAllocateInfo allocation_info
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = descriptor_pool,
        .descriptorSetCount = static_cast<uint32_t>(image_count),
        .pSetLayouts = layouts.data()
    };

    const VkResult sets_allocation = vkAllocateDescriptorSets(logical_device, &allocation_info, descriptor_sets.data());

    if (sets_allocation != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Allocation returned error code -> " + std::to_string(sets_allocation) + ".");

    std::vector<VkDescriptorImageInfo> descriptor_image_info(texture_image_views.size());

    for (int j = 0; j < texture_image_views.size(); j++)
    {
        /*
            - imageLayout / Defines the layout of the texture image.
            - imageView   / Describes how to access and treat the targeted texture image.
            - sampler     / Defines which texture sampler to use.
        */
        descriptor_image_info[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        descriptor_image_info[j].imageView = texture_image_views[j];
        descriptor_image_info[j].sampler = texture_sampler;
    }

    for (int i = 0; i < image_count; i++)
    {
        /*
            - buffer / Passes the uniform buffer to use.
            - offset / Defines from where we start to read/write.
            - range  / Defines the size in bytes to use.
        */
        VkDescriptorBufferInfo buffer_info
        {
            .buffer = uniform_buffers[i].buffer,
            .offset = 0,
            .range = VK_WHOLE_SIZE
        };

        std::vector<VkWriteDescriptorSet> write_sets(2);

        /*
            - sType           / Defines the type of the structure.
            - dstSet          / Defines the targeted descriptor set.
            - dstBinding      / Defines the descriptor binding of this descriptor set.
            - dstArrayElement / Defines the starting element of the array.
            - descriptorType  / Defines the type of the descriptor. Here, it's a uniform buffer descriptor.
            - descriptorCount / Defines the amount of descriptors to update.
            - pBufferInfo     / Passes all information of the buffer to use.
        */
        write_sets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write_sets[0].dstSet = descriptor_sets[i];
        write_sets[0].dstBinding = 0;
        write_sets[0].dstArrayElement = 0;
        write_sets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write_sets[0].descriptorCount = 1;
        write_sets[0].pBufferInfo = &buffer_info;

        /*
            - sType           / Defines the type of the structure.
            - dstSet          / Defines the targeted descriptor set.
            - dstBinding      / Defines the descriptor binding of this descriptor set.
            - dstArrayElement / Defines the starting element of the array.
            - descriptorType  / Defines the type of the descriptor. Here, it's a combined image sampler descriptor.
            - descriptorCount / Defines the amount of descriptors to update.
            - pImageInfo      / Passes all information about all texture images.
        */
        write_sets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write_sets[1].dstSet = descriptor_sets[i];
        write_sets[1].dstBinding = 1;
        write_sets[1].dstArrayElement = 0;
        write_sets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write_sets[1].descriptorCount = static_cast<uint32_t>(descriptor_image_info.size());
        write_sets[1].pImageInfo = descriptor_image_info.data();

        vkUpdateDescriptorSets(logical_device, static_cast<uint32_t>(write_sets.size()), write_sets.data(), 0, nullptr);
    }

    Utils::Logs::log("Done!", true);
    return descriptor_sets;
}



/*
    Cleanly destroy some descriptor sets.

    Tasks:
        1) Verify function parameters.
        2) Free the descriptor sets.
        3) Clear the vector list.

    Parameters:
        - descriptor_pool / VkDescriptorPool        / Descriptor pool of the Vulkan instance.
        - descriptor_sets / vector<VkDescriptorSet> / Descriptor sets to destroy.
        - logical_device  / VkDevice                / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Descriptors::destroy_descriptor_sets
(
    const VkDescriptorPool       &descriptor_pool,
    std::vector<VkDescriptorSet> &descriptor_sets,
    const VkDevice               &logical_device
)
{
    Utils::Logs::log("Destroying " + std::to_string(descriptor_sets.size()) + " descriptor sets.. ", false);

    if (descriptor_sets.size() < 1)
    {
        Utils::Logs::log("Done!", true);
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    vkFreeDescriptorSets(logical_device, descriptor_pool, descriptor_sets.size(), descriptor_sets.data());
    descriptor_sets.clear();

    Utils::Logs::log("Done!", true);
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Descriptors::descriptor_sets_handler::descriptor_sets_handler
(
    const VkDescriptorSetLayout          &descriptor_set_layout,
    const VkDescriptorPool               &descriptor_pool,
    const uint32_t                       &image_count,
    const VkDevice                       &logical_device,
    const std::vector<VkImageView>       &texture_image_views,
    const VkSampler                      &texture_sampler,
    const std::vector<UniformBufferInfo> &uniform_buffers
)
    : descriptor_pool(descriptor_pool), logical_device(logical_device)
{
    descriptor_sets = create_descriptor_sets(descriptor_set_layout, descriptor_pool, image_count, logical_device, texture_image_views, texture_sampler, uniform_buffers);
}

Descriptors::descriptor_sets_handler::~descriptor_sets_handler()
{
    destroy_descriptor_sets(descriptor_pool, descriptor_sets, logical_device);
}

std::vector<VkDescriptorSet> Descriptors::descriptor_sets_handler::get() const
{
    return descriptor_sets;
}
