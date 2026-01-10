#include "vulkan.descriptors.hpp"
#include "osge/utils/utils.hpp"
#include <libraries/vulkan/vulkan.h>
#include <string>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a descriptor set layout.
    Note: You should use the pre-made class to handle the descriptor set layout rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the parameters.
        2) Make the bindings for the shaders.
        3) Create the descriptor set layout.

    Parameters:
        - logical_device      / VkDevice            / Logical device of the Vulkan instance.
        - texture_image_views / vector<VkImageView> / Image views of the textures.

    Returns:
        The created descriptor set layout.
*/
VkDescriptorSetLayout Vulkan::Descriptors::create_descriptor_set_layout
(
    const VkDevice &logical_device,
    const std::vector<VkImageView> &texture_image_views
)
{
    Utils::Logs::log("Creating a descriptor set layout..");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Descriptor set layout creation failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    if (texture_image_views.size() < 1)
        Utils::Logs::crash_error_log("Descriptor set layout creation failed! No texture image views provided!");

    const VkDescriptorSetLayoutBinding uniform_buffer_binding
    {
        .binding = 0, // Index in the shader.
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT
    };

    const VkDescriptorSetLayoutBinding sampler_binding
    {
        .binding = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = static_cast<uint32_t>(texture_image_views.size()),
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
    };

    std::vector<VkDescriptorSetLayoutBinding> bindings = { uniform_buffer_binding, sampler_binding };

    const VkDescriptorSetLayoutCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = static_cast<uint32_t>(bindings.size()),
        .pBindings = bindings.data()
    };

    VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
    const VkResult layout_creation = vkCreateDescriptorSetLayout(logical_device, &create_info, nullptr, &descriptor_set_layout);

    if (layout_creation != VK_SUCCESS)
        Utils::Logs::crash_error_log("Descriptor set layout creation returned error code " + std::to_string(layout_creation) + ".");

    Utils::Logs::log("Descriptor set layout " + Utils::Text::get_memory_address(descriptor_set_layout) + " created successfully!");
    return descriptor_set_layout;
}



/*
    Cleanly destroy a descriptor set layout.

    Tasks:
        1) Verify the parameters.
        2) Destroy the descriptor set layout.
        3) Replace the objects addressess.

    Parameters:
        - descriptor_set_layout / VkDescriptorSetLayout / Descriptor set layout to destroy.
        - logical_device        / VkDevice              / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Vulkan::Descriptors::destroy_descriptor_set_layout
(
    VkDescriptorSetLayout &descriptor_set_layout,
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Destroying the " + Utils::Text::get_memory_address(descriptor_set_layout) + " descriptor set layout..");

    if (descriptor_set_layout == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Descriptor set layout destruction failed! The descriptor set layout provided (" + Utils::Text::get_memory_address(descriptor_set_layout) + ") is not valid!");
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Descriptor set layout destruction failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");
        return;
    }

    vkDestroyDescriptorSetLayout(logical_device, descriptor_set_layout, nullptr);
    descriptor_set_layout = VK_NULL_HANDLE;

    Utils::Logs::log("Descriptor set layout destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Vulkan::Descriptors::descriptor_set_layout_handler::descriptor_set_layout_handler
(
    const VkDevice &logical_device,
    const std::vector<VkImageView> &texture_image_views
)
    : logical_device(logical_device)
{
    descriptor_set_layout = Vulkan::Descriptors::create_descriptor_set_layout(logical_device, texture_image_views);
}

Vulkan::Descriptors::descriptor_set_layout_handler::~descriptor_set_layout_handler()
{
    Vulkan::Descriptors::destroy_descriptor_set_layout(descriptor_set_layout, logical_device);
}

VkDescriptorSetLayout Vulkan::Descriptors::descriptor_set_layout_handler::get() const
{
    return descriptor_set_layout;
}
