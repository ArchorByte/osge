#include "vulkan.images.hpp"
#include "osge/renderer/vulkan/buffers/vulkan.buffers.hpp"
#include "osge/utils/utils.hpp"
#include <libraries/vulkan/vulkan.h>
#include <utility>

/*
    Create an image.
    Warning: There is no class that will automatically destroy this image, you have to set one up yourself for memory safety reasons.

    Tasks:
        1) Verify the parameters.

    Parameters:
        - format          / VkFormat              / Format of the image.
        - height          / int                   / Height of the image.
        - image_tiling    / VkImageTiling         / Image tiling.
        - logical_device  / VkDevice              / Logical device of the Vulkan instance.
        - mip_levels      / uint32_t              / Mip levels used for LOD.
        - physical_device / VkPhysicalDevice      / Physical device used to run Vulkan.
        - samples_count   / VkSampleCountFlagBits / Amount of samples to render at the same time for multisampling.
        - usage_flags     / VkImageUsageFlags     / Usage flags.
        - width           / int                   / Width of the image.

    Returns:
        A pair containing the created image and its memory.
*/
std::pair<VkImage, VkDeviceMemory> Vulkan::Images::create_image
(
    const VkFormat &format,
    const int &height,
    const VkImageTiling &image_tiling,
    const VkDevice &logical_device,
    const uint32_t &mip_levels,
    const VkPhysicalDevice &physical_device,
    const VkSampleCountFlagBits &samples_count,
    const VkImageUsageFlags &usage_flags,
    const int &width
)
{
    if (height < 1)
        Utils::Logs::crash_error_log("Image creation failed! The image height provided (" + std::to_string(height) + ") is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Image creation failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    if (mip_levels < 1)
        Utils::Logs::crash_error_log("Image creation failed! The mip levels count provided (" + std::to_string(mip_levels) + ") is not valid!");

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Image creation failed! The physical device provided (" + Utils::Text::get_memory_address(physical_device) + ") is not valid!");

    if (width < 1)
        Utils::Logs::crash_error_log("Image creation failed! The image width provided (" + std::to_string(width) + ") is not valid!");

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
        Utils::Logs::crash_error_log("Image creation returned error code " + std::to_string(image_creation) + ".");

    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

    VkMemoryRequirements memory_requirements;
    vkGetImageMemoryRequirements(logical_device, image, &memory_requirements);

    const VkMemoryAllocateInfo allocation_info
    {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memory_requirements.size,
        .memoryTypeIndex = Vulkan::Buffers::find_memory_type(memory_properties, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    };

    VkDeviceMemory image_memory = VK_NULL_HANDLE;
    const VkResult memory_allocation = vkAllocateMemory(logical_device, &allocation_info, nullptr, &image_memory);

    if (memory_allocation != VK_SUCCESS)
        Utils::Logs::crash_error_log("Image creation failed! The memory allocation returned error code " + std::to_string(memory_allocation) + ".");

    vkBindImageMemory(logical_device, image, image_memory, 0);
    return { image, image_memory };
}
