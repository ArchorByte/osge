#include "texture.sampler.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <string>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a sampler for textures.
VkSampler create_vulkan_texture_sampler
(
    const VkPhysicalDevice &physical_device,
    const VkDevice &logical_device
)
{
    log("Creating a texture sampler..");

    if (!physical_device || physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Texture sampler creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Texture sampler creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    // Check if the anisotropy filter is supported.
    VkPhysicalDeviceFeatures device_features{};
    vkGetPhysicalDeviceFeatures(physical_device, &device_features);

    // Retrieve the physical device properties for the anisotropy filter.
    VkPhysicalDeviceProperties device_properties {};
    vkGetPhysicalDeviceProperties(physical_device, &device_properties);

    // Create info for the texture sampler.
    VkSamplerCreateInfo sampler_info {};
    sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_info.magFilter = VK_FILTER_LINEAR;
    sampler_info.minFilter = VK_FILTER_LINEAR;
    sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    sampler_info.unnormalizedCoordinates = VK_FALSE;
    sampler_info.compareEnable = VK_FALSE;
    sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
    sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler_info.mipLodBias = 0.0f;
    sampler_info.minLod = 0.0f;
    sampler_info.maxLod = 0.0f;

    if (device_features.samplerAnisotropy)
    {
        log("Anisotropy sampling features enabled!");
        sampler_info.anisotropyEnable = VK_TRUE;
        sampler_info.maxAnisotropy = device_properties.limits.maxSamplerAnisotropy;
    }
    else
    {
        // Disable anisotropy features if it's not supported by the physical device.
        sampler_info.anisotropyEnable = VK_FALSE;
        sampler_info.maxAnisotropy = 1.0f;
    }

    // Try to create the texture sampler.
    VkSampler texture_sampler = VK_NULL_HANDLE;
    VkResult sampler_creation = vkCreateSampler(logical_device, &sampler_info, nullptr, &texture_sampler);

    if (sampler_creation != VK_SUCCESS)
    {
        fatal_error_log("Texture sampler creation returned error code " + std::to_string(sampler_creation) + ".");
    }

    if (!texture_sampler || texture_sampler == VK_NULL_HANDLE)
    {
        fatal_error_log("Texture sampler creation output (" + force_string(texture_sampler) + ") is not valid!");
    }

    log("Texture sampler " + force_string(texture_sampler) + " created successfully!");
    return texture_sampler;
}

// Destroy a texture sampler.
void destroy_vulkan_texture_sampler
(
    const VkDevice &logical_device,
    VkSampler texture_sampler
)
{
    log("Destroying the " + force_string(texture_sampler) + " texture sampler..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        error_log("Texture sampler destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (!texture_sampler || texture_sampler == VK_NULL_HANDLE)
    {
        error_log("Texture sampler destruction failed! The texture sampler provided (" + force_string(texture_sampler) + ") is not valid!");
        return;
    }

    // Destroy the texture sampler and clear the address.
    vkDestroySampler(logical_device, texture_sampler, nullptr);
    texture_sampler = VK_NULL_HANDLE;

    log("Texture sampler destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_TextureSampler::Vulkan_TextureSampler
(
    const VkPhysicalDevice &physical_device,
    const VkDevice &logical_device
) : logical_device(logical_device)
{
    texture_sampler = create_vulkan_texture_sampler(physical_device, logical_device);
}

// Destructor.
Vulkan_TextureSampler::~Vulkan_TextureSampler()
{
    destroy_vulkan_texture_sampler(logical_device, texture_sampler);
}

VkSampler Vulkan_TextureSampler::get() const
{
    return texture_sampler;
}
