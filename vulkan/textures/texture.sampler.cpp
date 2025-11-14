#include "texture.sampler.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

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

    if (physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Texture sampler creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Texture sampler creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    // Check if the anisotropy filter is supported.
    VkPhysicalDeviceFeatures device_features {};
    vkGetPhysicalDeviceFeatures(physical_device, &device_features);

    // Retrieve the physical device properties for the anisotropy filter.
    VkPhysicalDeviceProperties device_properties {};
    vkGetPhysicalDeviceProperties(physical_device, &device_properties);

    VkSamplerCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = VK_FILTER_LINEAR, // Set the magnification filter.
        .minFilter = VK_FILTER_LINEAR, // Set the minification filter.
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT, // Address mode for the X-axis.
        .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT, // Address mode for the Y-axis.
        .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT, // Address mode for the Z-axis.
        .mipLodBias = 0.0f,
        .compareEnable = VK_FALSE, // Disable compare features.
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .minLod = 0.0f,
        .maxLod = 0.0f,
        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates = VK_FALSE // Normalize coordinates.
    };

    if (device_features.samplerAnisotropy)
    {
        log("Enabled anisotropy sampling features!");
        create_info.anisotropyEnable = VK_TRUE;
        create_info.maxAnisotropy = device_properties.limits.maxSamplerAnisotropy;
    }
    else // Disable anisotropy features if it's not supported by the physical device.
    {
        create_info.anisotropyEnable = VK_FALSE;
        create_info.maxAnisotropy = 1.0f;
    }

    VkSampler texture_sampler = VK_NULL_HANDLE;
    const VkResult sampler_creation = vkCreateSampler(logical_device, &create_info, nullptr, &texture_sampler);

    if (sampler_creation != VK_SUCCESS)
    {
        fatal_error_log("Texture sampler creation returned error code " + std::to_string(sampler_creation) + ".");
    }

    if (texture_sampler == VK_NULL_HANDLE)
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

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Texture sampler destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (texture_sampler == VK_NULL_HANDLE)
    {
        error_log("Texture sampler destruction failed! The texture sampler provided (" + force_string(texture_sampler) + ") is not valid!");
        return;
    }

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
