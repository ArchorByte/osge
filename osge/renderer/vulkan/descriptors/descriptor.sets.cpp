#include "vulkan.descriptors.hpp"
#include "osge/renderer/vulkan/buffers/vulkan.buffers.hpp"
#include "osge/utils/utils.hpp"
#include <libraries/vulkan/vulkan.h>
#include <string>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a descriptor set for each swap chain image.
    Note: You should use the pre-made class to handle the descriptor sets rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the parameters.

    Parameters:
        - descriptor_set_layout / VkDescriptorSetLayout     / Layout to use for the descriptor sets.
        - descriptor_pool       / VkDescriptorPool          / Descriptor pool of the Vulkan instance.
        - image_count           / uint32_t                  / Amount of descriptor sets to create.
        - logical_device        / VkDevice                  / Logical device of the Vulkan instance.
        - texture_image_views   / vector<VkImageView>       / Image views of the textures.
        - texture_sampler       / VkSampler                 / Texture sampler of the Vulkan instance.
        - uniform_buffers       / vector<UniformBufferInfo> / Uniform buffers of the Vulkan instance.

    Returns:
        A vector list containing all created descriptor sets.
*/
std::vector<VkDescriptorSet> Vulkan::Descriptors::create_descriptor_sets
(
    const VkDescriptorSetLayout &descriptor_set_layout,
    const VkDescriptorPool &descriptor_pool,
    const uint32_t &image_count,
    const VkDevice &logical_device,
    const std::vector<VkImageView> &texture_image_views,
    const VkSampler &texture_sampler,
    const std::vector<UniformBufferInfo> &uniform_buffers
)
{
    Utils::Logs::log("Creating " + std::to_string(image_count) + " descriptor sets..");

    if (descriptor_set_layout == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Descriptor sets creation failed! The descriptor set layout provided (" + Utils::Text::get_memory_address(descriptor_set_layout) + ") is not valid!");

    if (descriptor_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Descriptor sets creation failed! The descriptor pool provided (" + Utils::Text::get_memory_address(descriptor_pool) + ") is not valid!");

    if (image_count < 1)
        Utils::Logs::crash_error_log("Descriptor sets creation failed! The images count provided (" + std::to_string(image_count) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Descriptor sets creation failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    if (texture_image_views.size() < 1)
        Utils::Logs::crash_error_log("Descriptor sets creation failed! No texture image views provided!");

    if (texture_sampler == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Descriptor sets creation failed! The texture sampler provided (" + Utils::Text::get_memory_address(texture_sampler) + ") is not valid!");

    if (uniform_buffers.size() < 1)
        Utils::Logs::crash_error_log("Descriptor sets creation failed! No uniform buffers provided!");

    std::vector<VkDescriptorSet> descriptor_sets(image_count);
    std::vector<VkDescriptorSetLayout> layouts(image_count, descriptor_set_layout);

    VkDescriptorSetAllocateInfo allocation_info
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = descriptor_pool,
        .descriptorSetCount = static_cast<uint32_t>(image_count),
        .pSetLayouts = layouts.data()
    };

    const VkResult sets_allocation = vkAllocateDescriptorSets(logical_device, &allocation_info, descriptor_sets.data());

    if (sets_allocation != VK_SUCCESS)
        Utils::Logs::crash_error_log("Descriptor sets creation failed! Descriptor sets allocation returned error code " + std::to_string(sets_allocation) + ".");

    for (int i = 0; i < image_count; i++)
    {
        VkDescriptorBufferInfo buffer_info
        {
            .buffer = uniform_buffers[i].buffer,
            .offset = 0,
            .range = sizeof(UniformBufferObject)
        };

        std::vector<VkWriteDescriptorSet> write_sets(2);
        std::vector<VkDescriptorImageInfo> descriptor_image_info(texture_image_views.size());

        write_sets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write_sets[0].dstSet = descriptor_sets[i];
        write_sets[0].dstBinding = 0;
        write_sets[0].dstArrayElement = 0;
        write_sets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write_sets[0].descriptorCount = 1;
        write_sets[0].pBufferInfo = &buffer_info;

        for (int j = 0; j < texture_image_views.size(); j++)
        {
            descriptor_image_info[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            descriptor_image_info[j].imageView = texture_image_views[j];
            descriptor_image_info[j].sampler = texture_sampler;
        }

        write_sets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write_sets[1].dstSet = descriptor_sets[i];
        write_sets[1].dstBinding = 1;
        write_sets[1].dstArrayElement = 0;
        write_sets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write_sets[1].descriptorCount = static_cast<uint32_t>(descriptor_image_info.size());
        write_sets[1].pImageInfo = descriptor_image_info.data();

        vkUpdateDescriptorSets(logical_device, static_cast<uint32_t>(write_sets.size()), write_sets.data(), 0, nullptr);
        Utils::Logs::log("- Descriptor set #" + std::to_string(i + 1) + "/" + std::to_string(descriptor_sets.size()) + " created successfully!");
    }

    Utils::Logs::log(std::to_string(descriptor_sets.size()) + " descriptor sets created successfully!");
    return descriptor_sets;
}



/*
    Cleanly destroy some descriptor sets.

    Tasks:
        1) Verify the parameters.
        2) Free the descriptor sets.
        3) Clear the vector list.

    Parameters:
        - descriptor_pool / VkDescriptorPool        / Descriptor pool of the Vulkan instance.
        - descriptor_sets / vector<VkDescriptorSet> / Descriptor sets to destroy.
        - logical_device  / VkDevice                / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Vulkan::Descriptors::destroy_descriptor_sets
(
    const VkDescriptorPool &descriptor_pool,
    std::vector<VkDescriptorSet> &descriptor_sets,
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Destroying " + std::to_string(descriptor_sets.size()) + " descriptor sets..");

    if (descriptor_sets.size() < 1)
    {
        Utils::Logs::error_log("Descriptor sets destruction failed! No descriptor sets provided!");
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Descriptor sets destruction failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");
        return;
    }

    vkFreeDescriptorSets(logical_device, descriptor_pool, descriptor_sets.size(), descriptor_sets.data());
    descriptor_sets.clear();

    Utils::Logs::log("Descriptor sets destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Vulkan::Descriptors::descriptor_sets_handler::descriptor_sets_handler
(
    const VkDescriptorSetLayout &descriptor_set_layout,
    const VkDescriptorPool &descriptor_pool,
    const uint32_t &image_count,
    const VkDevice &logical_device,
    const std::vector<VkImageView> &texture_image_views,
    const VkSampler &texture_sampler,
    const std::vector<UniformBufferInfo> &uniform_buffers
)
    : descriptor_pool(descriptor_pool), logical_device(logical_device)
{
    descriptor_sets = Vulkan::Descriptors::create_descriptor_sets(descriptor_set_layout, descriptor_pool, image_count, logical_device, texture_image_views, texture_sampler, uniform_buffers);
}

Vulkan::Descriptors::descriptor_sets_handler::~descriptor_sets_handler()
{
    Vulkan::Descriptors::destroy_descriptor_sets(descriptor_pool, descriptor_sets, logical_device);
}

std::vector<VkDescriptorSet> Vulkan::Descriptors::descriptor_sets_handler::get() const
{
    return descriptor_sets;
}
