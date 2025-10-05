#include "descriptor.sets.hpp"

#include "../uniform/uniform.buffers.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>
#include <string>

// Create a descriptor set for each swap chain image.
std::vector<VkDescriptorSet> create_vulkan_descriptor_sets
(
    const VkDevice &logical_device,
    const uint32_t &images_count,
    const VkDescriptorSetLayout &descriptor_set_layout,
    const VkDescriptorPool &descriptor_pool,
    const std::vector<UniformBufferInfo> &uniform_buffers,
    const std::vector<VkImageView> &texture_image_views,
    const VkSampler &texture_sampler
)
{
    log("Creating " + std::to_string(images_count) + " descriptor sets..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Descriptor sets creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (images_count < 1)
    {
        fatal_error_log("Descriptor sets creation failed! The images count provided (" + std::to_string(images_count) + ") is not valid!");
    }

    if (descriptor_set_layout == VK_NULL_HANDLE)
    {
        fatal_error_log("Descriptor sets creation failed! The descriptor set layout provided (" + force_string(descriptor_set_layout) + ") is not valid!");
    }

    if (descriptor_pool == VK_NULL_HANDLE)
    {
        fatal_error_log("Descriptor sets creation failed! The descriptor pool provided (" + force_string(descriptor_pool) + ") is not valid!");
    }

    if (uniform_buffers.size() < 1)
    {
        fatal_error_log("Descriptor sets creation failed! No uniform buffers were provided!");
    }

    if (texture_image_views.size() < 1)
    {
        fatal_error_log("Descriptor sets creation failed! No texture image views were provided!");
    }

    if (texture_sampler == VK_NULL_HANDLE)
    {
        fatal_error_log("Descriptor sets creation failed! The texture sampler provided (" + force_string(texture_sampler) + ") is not valid!");
    }

    std::vector<VkDescriptorSet> descriptor_sets(images_count);
    std::vector<VkDescriptorSetLayout> layouts(images_count, descriptor_set_layout); // Duplicate 'images count' times the descriptor set layout.

    VkDescriptorSetAllocateInfo allocation_info {};
    allocation_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocation_info.descriptorPool = descriptor_pool;                         // Pass the descriptor pool.
    allocation_info.descriptorSetCount = static_cast<uint32_t>(images_count); // Amount of descriptor sets to pass.
    allocation_info.pSetLayouts = layouts.data();                             // Pass the descriptor set layouts.

    VkResult sets_allocation = vkAllocateDescriptorSets(logical_device, &allocation_info, descriptor_sets.data());

    if (sets_allocation != VK_SUCCESS)
    {
        fatal_error_log("Descriptor sets creation failed! Descriptor sets allocation returned error code " + std::to_string(sets_allocation) + ".");
    }

    for (int i = 0; i < images_count; i++)
    {
        VkDescriptorBufferInfo buffer_info {};
        buffer_info.buffer = uniform_buffers[i].buffer;  // Pass the uniform buffer.
        buffer_info.offset = 0;                          // Start reading at the start of the buffer.
        buffer_info.range = sizeof(UniformBufferObject); // Pass the size of the buffer.

        std::vector<VkWriteDescriptorSet> write_sets(2);
        std::vector<VkDescriptorImageInfo> descriptor_image_info(texture_image_views.size());

        write_sets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write_sets[0].dstSet = descriptor_sets[i];                        // Pass the descriptor set.
        write_sets[0].dstBinding = 0;                                     // Binding index in the shader.
        write_sets[0].dstArrayElement = 0;                                // Update the first element of the array.
        write_sets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; // Descriptor for a uniform buffer.
        write_sets[0].descriptorCount = 1;                                // Amount of descriptors to update.
        write_sets[0].pBufferInfo = &buffer_info;                         // Pass the buffer info.

        // Create a descriptor image view for each texture image views.
        for (int j = 0; j < texture_image_views.size(); j++)
        {
            descriptor_image_info[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            descriptor_image_info[j].imageView = texture_image_views[j]; // Texture image views to pass.
            descriptor_image_info[j].sampler = texture_sampler;          // Pass the texture sampler.
        }

        write_sets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write_sets[1].dstSet = descriptor_sets[i];
        write_sets[1].dstBinding = 1;
        write_sets[1].dstArrayElement = 0;
        write_sets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // Descriptor for an image sampler.
        write_sets[1].descriptorCount = static_cast<uint32_t>(descriptor_image_info.size());
        write_sets[1].pImageInfo = descriptor_image_info.data();

        vkUpdateDescriptorSets(logical_device, static_cast<uint32_t>(write_sets.size()), write_sets.data(), 0, nullptr);
        log("- Descriptor set #" + std::to_string(i + 1) + "/" + std::to_string(descriptor_sets.size()) + " created successfully!");
    }

    log(std::to_string(descriptor_sets.size()) + " descriptor sets created successfully!");
    return descriptor_sets;
}
