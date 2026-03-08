#include "vulkan.descriptors.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <string>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a descriptor set layout.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Make bindings for the shaders.
        3) Create the descriptor set layout.

    Parameters:
        - logical_device      / VkDevice            / Logical device of the Vulkan instance.
        - texture_image_views / vector<VkImageView> / Describes how to access and treat the texture images.

    Returns:
        The created descriptor set layout.
*/
VkDescriptorSetLayout Descriptors::create_descriptor_set_layout
(
    const VkDevice                 &logical_device,
    const std::vector<VkImageView> &texture_image_views
)
{
    Utils::Logs::log("Creating descriptor set layout.. ", false);

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (texture_image_views.size() < 1)
        Utils::Logs::crash_log("Failed! No texture image views provided!");

    /*
        - binding         / Defines the number identifying this binding.
        - descriptorType  / Defines which type of resource descriptors are used for the binding. Here, it's for uniform buffers.
        - descriptorCount / Defines the amount of descriptors in the binding.
        - stageFlags      / Defines which shader stage can access to this binding.
    */
    const VkDescriptorSetLayoutBinding uniform_buffer_binding
    {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT
    };

    /*
        - binding         / Defines the number identifying this binding.
        - descriptorType  / Defines which type of resource descriptors are used for the binding. Here, it's for an image sampler.
        - descriptorCount / Defines the amount of descriptors in the binding.
        - stageFlags      / Defines which shader stage can access to this binding.
    */
    const VkDescriptorSetLayoutBinding sampler_binding
    {
        .binding = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = static_cast<uint32_t>(texture_image_views.size()),
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
    };

    std::vector<VkDescriptorSetLayoutBinding> bindings = { uniform_buffer_binding, sampler_binding };

    /*
        - sType        / Defines the type of the structure.
        - bindingCount / Defines the amount of bindings to pass.
        - pBindings    / Passes the bindings.
    */
    const VkDescriptorSetLayoutCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = static_cast<uint32_t>(bindings.size()),
        .pBindings = bindings.data()
    };

    VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
    const VkResult layout_creation = vkCreateDescriptorSetLayout(logical_device, &create_info, nullptr, &descriptor_set_layout);

    if (layout_creation != VK_SUCCESS)
        Utils::Logs::crash_log("Descriptor set layout creation returned error code " + std::to_string(layout_creation) + ".");

    Utils::Logs::log("Done! Memory address -> " + Utils::Text::get_memory_address(descriptor_set_layout) + ".", true);
    return descriptor_set_layout;
}



/*
    Cleanly destroy a descriptor set layout.

    Tasks:
        1) Verify function parameters.
        2) Destroy the descriptor set layout.
        3) Set object to null.

    Parameters:
        - descriptor_set_layout / VkDescriptorSetLayout / Descriptor set layout to destroy.
        - logical_device        / VkDevice              / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Descriptors::destroy_descriptor_set_layout
(
    VkDescriptorSetLayout &descriptor_set_layout,
    const VkDevice        &logical_device
)
{
    Utils::Logs::log("Destroying descriptor set layout (" + Utils::Text::get_memory_address(descriptor_set_layout) + ").. ", false);

    if (descriptor_set_layout == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Descriptor set layout invalid.", true);
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    vkDestroyDescriptorSetLayout(logical_device, descriptor_set_layout, nullptr);
    descriptor_set_layout = VK_NULL_HANDLE;

    Utils::Logs::log("Done!", true);
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Descriptors::descriptor_set_layout_handler::descriptor_set_layout_handler
(
    const VkDevice                 &logical_device,
    const std::vector<VkImageView> &texture_image_views
)
    : logical_device(logical_device)
{
    descriptor_set_layout = create_descriptor_set_layout(logical_device, texture_image_views);
}

Descriptors::descriptor_set_layout_handler::~descriptor_set_layout_handler()
{
    destroy_descriptor_set_layout(descriptor_set_layout, logical_device);
}

VkDescriptorSetLayout Descriptors::descriptor_set_layout_handler::get() const
{
    return descriptor_set_layout;
}
