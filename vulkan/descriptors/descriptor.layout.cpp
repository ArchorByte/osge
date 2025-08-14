#include "layout.hpp"

#include "../../../logs/terminal.hpp"
#include "../../../tools/text_format.hpp"

#include <vulkan/vulkan.h>
#include <string>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a descriptor set layout.
VkDescriptorSetLayout create_vulkan_descriptor_set_layout
(
    const VkDevice &logical_device
)
{
    log("Creating a descriptor set layout..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Descriptor set layout creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    // Create info for the descriptor set layout binding.
    VkDescriptorSetLayoutBinding binding {};
    binding.binding = 0;                                        // The binding index in the shader.
    binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; // Use this binding for a uniform buffer.
    binding.descriptorCount = 1;                                // Amount of descriptors in the binding.
    binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;            // Allow vertex shader stages to access this binding.
    binding.pImmutableSamplers = nullptr;                       // We don't need it.

    // Create info for the descriptor set layout.
    VkDescriptorSetLayoutCreateInfo layout_info {};
    layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = 1;     // Amount of bindings to pass.
    layout_info.pBindings = &binding; // Pass the binding.

    VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
    VkResult layout_creation = vkCreateDescriptorSetLayout(logical_device, &layout_info, nullptr, &descriptor_set_layout); // Try to create the layout.

    if (layout_creation != VK_SUCCESS)
    {
        fatal_error_log("Descriptor set layout creation returned error code " + std::to_string(layout_creation) + ".");
    }

    if (!descriptor_set_layout || descriptor_set_layout == VK_NULL_HANDLE)
    {
        fatal_error_log("Descriptor set layout creation output \"" + force_string(descriptor_set_layout) + "\" is not valid!");
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

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        error_log("Descriptor set layout destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (!descriptor_set_layout || descriptor_set_layout == VK_NULL_HANDLE)
    {
        error_log("Descriptor set layout destruction failed! The descriptor set layout provided (" + force_string(descriptor_set_layout) + ") is not valid!");
        return;
    }

    // Destroy the descriptor set layout and dispose of the address.
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
    const VkDevice &logical_device
) : logical_device(logical_device)
{
    descriptor_set_layout = create_vulkan_descriptor_set_layout(logical_device);
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
