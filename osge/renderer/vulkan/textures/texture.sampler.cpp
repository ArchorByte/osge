#include "vulkan.textures.hpp"

#include "libraries/vulkan/vulkan.h"
#include "../../../utils/utils.hpp"

#include <string>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a texture sampler.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Retrieve the features and properties of the physical device.
        3) Enable anisotropy features if supported.
        4) Create texture sampler.

    Parameters:
        - logical_device  / VkDevice         / Logical device of the Vulkan instance.
        - physical_device / VkPhysicalDevice / Physical device used to run this Vulkan instance.

    Returns:
        The created texture sampler.
*/
VkSampler Textures::create_texture_sampler
(
    const VkDevice         &logical_device,
    const VkPhysicalDevice &physical_device
)
{
    Utils::Logs::log("Creating texture sampler.. ", false);

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical device invalid.");

    VkPhysicalDeviceFeatures device_features {};
    vkGetPhysicalDeviceFeatures(physical_device, &device_features);

    VkPhysicalDeviceProperties device_properties {};
    vkGetPhysicalDeviceProperties(physical_device, &device_properties);

    /*
        - sType                   / Defines the type of the structure.
        - magFilter               / Defines the magnification filter to apply.
        - minFilter               / Defines the minification filter to apply.
        - mipmapMode              / Mip map filter to apply.
        - addressModeU            / Defines wrapping operation used on X axis, if out of bounds.
        - addressModeV            / Defines wrapping operation used on Y axis, if out of bounds.
        - addressModeW            / Defines wrapping operation used on Z axis, if out of bounds.
        - compareEnable           / Defines whether comparison against reference value is enabled or not.
        - compareOp               / Defines which comparison operator we are going to use.
        - maxLod                  / Defines the maximum LOD (Level of Details) value.
        - borderColor             / Defines the color of the predefined border.
        - unnormalizedCoordinates / Defines whether we use unnormalized or normalized texel coordinates.
    */
    VkSamplerCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = VK_FILTER_LINEAR,
        .minFilter = VK_FILTER_LINEAR,
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .maxLod = VK_LOD_CLAMP_NONE,
        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates = VK_FALSE
    };

    if (device_features.samplerAnisotropy)
    {
        /*
            - anisotropyEnable / Defines whether we enable anisotropy or not.
            - maxAnisotropy    / Defines the maximum anisotropy value.
        */
        create_info.anisotropyEnable = VK_TRUE;
        create_info.maxAnisotropy = device_properties.limits.maxSamplerAnisotropy;
    }
    else
    {
        /*
            - anisotropyEnable / Defines whether we enable anisotropy or not.
            - maxAnisotropy    / Defines the maximum anisotropy value.
        */
        create_info.anisotropyEnable = VK_FALSE;
        create_info.maxAnisotropy = 1.0f;
    }

    VkSampler texture_sampler = VK_NULL_HANDLE;
    const VkResult sampler_creation = vkCreateSampler(logical_device, &create_info, nullptr, &texture_sampler);

    if (sampler_creation != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Creation returned error code -> " + std::to_string(sampler_creation) + ".");

    Utils::Logs::log("Done! Memory address -> " + Utils::Text::get_memory_address(texture_sampler) + ".", true);
    return texture_sampler;
}



/*
    Destroy a texture sampler.

    Tasks:
        1) Verify function parameters.
        2) Destroy texture sampler.
        3) Set object to null.

    Parameters:
        - logical_device  / VkDevice  / Logical device of the Vulkan instance.
        - texture_sampler / VkSampler / Texture sampler to destroy.

    Returns:
        No object returned.
*/
void Textures::destroy_texture_sampler
(
    const VkDevice &logical_device,
    VkSampler      &texture_sampler
)
{
    Utils::Logs::log("Destroying " + Utils::Text::get_memory_address(texture_sampler) + " texture sampler.. ", false);

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    if (texture_sampler == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Texture sampler invalid.", true);
        return;
    }

    vkDestroySampler(logical_device, texture_sampler, nullptr);
    texture_sampler = VK_NULL_HANDLE;

    Utils::Logs::log("Done!", true);
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Textures::texture_sampler_handler::texture_sampler_handler
(
    const VkDevice         &logical_device,
    const VkPhysicalDevice &physical_device
)
    : logical_device(logical_device)
{
    texture_sampler = create_texture_sampler(logical_device, physical_device);
}

Textures::texture_sampler_handler::~texture_sampler_handler()
{
    destroy_texture_sampler(logical_device, texture_sampler);
}

VkSampler Textures::texture_sampler_handler::get() const
{
    return texture_sampler;
}
