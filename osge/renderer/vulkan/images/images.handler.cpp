#include "vulkan.images.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/renderer/vulkan/vulkan.renderer.hpp"
#include "osge/utils/utils.hpp"

#include <utility>

/*
    Create a Vulkan image.
    Warning: There is no class that will automatically destroy this image, you have to set one up yourself for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Create Vulkan image.
        3) Allocate memory to the created image.
        4) Bind memory and image.

    Parameters:
        - format          / VkFormat              / Defines the format to use for the image to create.
        - height          / int                   / Defines the height of the image.
        - image_tiling    / VkImageTiling         / Defines the tiling arrangement of data for the image.
        - logical_device  / VkDevice              / Logical device of this Vulkan instance.
        - mip_levels      / uint32_t              / Mip levels used for Level Of Details (LOD).
        - physical_device / VkPhysicalDevice      / Physical device used to run this Vulkan instance.
        - samples_count   / VkSampleCountFlagBits / Amount of samples to use for multisampling.
        - usage_flags     / VkImageUsageFlags     / Defines to Vulkan what we are going to do with this image.
        - width           / int                   / Defines the width of the image.

    Returns:
        A pair containing the created image and its memory.
*/
std::pair<VkImage, VkDeviceMemory> Images::create_image
(
    const VkFormat              &format,
    const int                   &height,
    const VkImageTiling         &image_tiling,
    const VkDevice              &logical_device,
    const uint32_t              &mip_levels,
    const VkPhysicalDevice      &physical_device,
    const VkSampleCountFlagBits &samples_count,
    const VkImageUsageFlags     &usage_flags,
    const int                   &width
)
{
    Utils::Logs::log("Creating Vulkan image.. ", false);

    if (height < 1)
        Utils::Logs::crash_log("Failed! Image height invalid -> " + std::to_string(height) + ".");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (mip_levels < 1)
        Utils::Logs::crash_log("Failed! Mip levels count invalid -> " + std::to_string(mip_levels) + ".");

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical device invalid.");

    if (width < 1)
        Utils::Logs::crash_log("Failed! Image width invalid -> " + std::to_string(width) + ".");

    /*
        - sType         / Defines the type of the structure.
        - imageType     / Defines the type of the image to create.
        - format        / Defines the format to use for the image to create.
        - extent        / Defines the resolution of the image.
            - width     / Defines width.
            - height    / Defines height.
            - depth     / Defines depth.
        - mipLevels     / Mip levels used for Level Of Details (LOD).
        - arrayLayers   / Defines the amount of array layers in the image.
        - samples       / Defines the amount of samples in the image.
        - tiling        / Defines the tiling arrangement of data for the image.
        - usage         / Defines to Vulkan what we are going to do with this image.
        - sharingMode   / Defines if whether the image is sharable or not between queue families. Here, disallowed.
        - initialLayout / Defines the first image layout we use.
    */
    const VkImageCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = format,
        .extent =
        {
            .width = static_cast<uint32_t>(width),
            .height = static_cast<uint32_t>(height),
            .depth = 1,
        },
        .mipLevels = mip_levels,
        .arrayLayers = 1,
        .samples = samples_count,
        .tiling = image_tiling,
        .usage = usage_flags,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
    };

    VkImage image = VK_NULL_HANDLE;
    const VkResult image_creation = vkCreateImage(logical_device, &create_info, nullptr, &image);

    if (image_creation != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Creation returned error code -> " + std::to_string(image_creation) + ".");

    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

    VkMemoryRequirements memory_requirements;
    vkGetImageMemoryRequirements(logical_device, image, &memory_requirements);

    /*
        - sType           / Defines the type of the structure.
        - allocationSize  / Amount of bytes to allocate.
        - memoryTypeIndex / Index identifying a memory type from the memory properties.
    */
    const VkMemoryAllocateInfo allocation_info
    {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memory_requirements.size,
        .memoryTypeIndex = Vulkan::Buffers::find_memory_type(memory_properties, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    };

    VkDeviceMemory image_memory = VK_NULL_HANDLE;
    const VkResult memory_allocation = vkAllocateMemory(logical_device, &allocation_info, nullptr, &image_memory);

    if (memory_allocation != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Memory allocation returned error code -> " + std::to_string(memory_allocation) + ".");

    vkBindImageMemory(logical_device, image, image_memory, 0);
    Utils::Logs::log("Done!", true);

    return { image, image_memory };
}
