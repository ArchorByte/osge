#include "descriptor.set.layout.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a descriptor set layout.
VkDescriptorSetLayout create_vulkan_descriptor_set_layout
(
    const VkDevice &logical_device,
    const std::vector<VkImageView> &texture_image_views
)
{
    log("Creating a descriptor set layout..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Descriptor set layout creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (texture_image_views.size() < 1)
    {
        fatal_error_log("Descriptor set layout creation failed! No texture image views were provided!");
    }

    VkDescriptorSetLayoutBinding uniform_buffer_binding
    {
        .binding = 0,                                        // Binding index in the shader.
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, // Use this binding for a uniform buffer.
        .descriptorCount = 1,                                // Amount of descriptors in the binding.
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT             // Allow vertex shader stages to access to this binding.
    };

    VkDescriptorSetLayoutBinding sampler_binding
    {
        .binding = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, // Use this binding for an image sampler.
        .descriptorCount = static_cast<uint32_t>(texture_image_views.size()),
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT // Allow stage fragment shader stages to access to this binding.
    };

    // Merge the two bindings into one vector list.
    std::vector<VkDescriptorSetLayoutBinding> bindings = { uniform_buffer_binding, sampler_binding };

    VkDescriptorSetLayoutCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = static_cast<uint32_t>(bindings.size()), // Amount of bindings to pass.
        .pBindings = bindings.data()                            // Pass the bindings.
    };

    VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
    const VkResult layout_creation = vkCreateDescriptorSetLayout(logical_device, &create_info, nullptr, &descriptor_set_layout);

    if (layout_creation != VK_SUCCESS)
    {
        fatal_error_log("Descriptor set layout creation returned error code " + std::to_string(layout_creation) + ".");
    }

    if (descriptor_set_layout == VK_NULL_HANDLE)
    {
        fatal_error_log("Descriptor set layout creation output (" + force_string(descriptor_set_layout) + ") is not valid!");
    }

    log("Descriptor set layout " + force_string(descriptor_set_layout) + " created successfully!");
    return descriptor_set_layout;
}

// Destroy a descriptor set layout.
void destroy_vulkan_descriptor_set_layout
(
    const VkDevice &logical_device,
    VkDescriptorSetLayout &descriptor_set_layout
)
{
    log("Destroying the " + force_string(descriptor_set_layout) + " descriptor set layout..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Descriptor set layout destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (descriptor_set_layout == VK_NULL_HANDLE)
    {
        error_log("Descriptor set layout destruction failed! The descriptor set layout provided (" + force_string(descriptor_set_layout) + ") is not valid!");
        return;
    }

    vkDestroyDescriptorSetLayout(logical_device, descriptor_set_layout, nullptr);
    descriptor_set_layout = VK_NULL_HANDLE;

    log("Descriptor set layout destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_DescriptorSetLayout::Vulkan_DescriptorSetLayout
(
    const VkDevice &logical_device,
    const std::vector<VkImageView> &texture_image_views
) : logical_device(logical_device)
{
    descriptor_set_layout = create_vulkan_descriptor_set_layout(logical_device, texture_image_views);
}

// Destructor.
Vulkan_DescriptorSetLayout::~Vulkan_DescriptorSetLayout()
{
    destroy_vulkan_descriptor_set_layout(logical_device, descriptor_set_layout);
}

VkDescriptorSetLayout Vulkan_DescriptorSetLayout::get() const
{
    return descriptor_set_layout;
}
