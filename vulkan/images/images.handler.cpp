#include "images.handler.hpp"

#include "../buffers/buffers.memory.hpp"
#include "../../utils/tool.text.format.hpp"
#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>
#include <utility>

// Create an image and bind it to some memory.
std::pair<VkImage, VkDeviceMemory> create_image
(
    const VkPhysicalDevice &physical_device,
    const VkDevice &logical_device,
    const int &width,
    const int &height,
    const VkFormat &format,
    const VkImageTiling &tiling,
    const VkImageUsageFlags &usage_flags
)
{
    if (physical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Image creation failed! The physical device provided (" + force_string(physical_device) + ") is not valid!");
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Image creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (width < 1)
    {
        fatal_error_log("Image creation failed! The image width provided (" + std::to_string(width) + ") is not valid!");
    }

    if (height < 1)
    {
        fatal_error_log("Image creation failed! The image height provided (" + std::to_string(height) + ") is not valid!");
    }

    const VkImageCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = format,
        .extent =
        {
            .width = static_cast<uint32_t>(width),   // Pass the image width.
            .height = static_cast<uint32_t>(height), // Pass the image height.
            .depth = 1,                              // Select the image depth.
        },
        .mipLevels = 1,                                                        // Amount of mit maps.
        .arrayLayers = 1,                                                      // Amount of array layers.
        .samples = VK_SAMPLE_COUNT_1_BIT,                                      // Disable multisampling.
        .tiling = tiling,
        .usage = usage_flags,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,                              // That texture image is not shared by queues.
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
    };

    VkImage image = VK_NULL_HANDLE;
    const VkResult image_creation = vkCreateImage(logical_device, &create_info, nullptr, &image);

    if (image_creation != VK_SUCCESS)
    {
        fatal_error_log("Image creation returned error code " + std::to_string(image_creation) + ".");
    }

    if (image == VK_NULL_HANDLE)
    {
        fatal_error_log("Image creation output (" + force_string(image) + ") is not valid!");
    }

    // Get the memory properties of the physical device.
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

    // Get the memory requirements of the texture image.
    VkMemoryRequirements memory_requirements;
    vkGetImageMemoryRequirements(logical_device, image, &memory_requirements);

    const VkMemoryAllocateInfo allocation_info
    {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memory_requirements.size,
        .memoryTypeIndex = find_memory_type(memory_requirements.memoryTypeBits, memory_properties, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    };

    VkDeviceMemory image_memory = VK_NULL_HANDLE;
    const VkResult memory_allocation = vkAllocateMemory(logical_device, &allocation_info, nullptr, &image_memory);

    if (memory_allocation != VK_SUCCESS)
    {
        fatal_error_log("Image creation failed! The memory allocation returned error code " + std::to_string(memory_allocation) + ".");
    }

    if (image_memory == VK_NULL_HANDLE)
    {
        fatal_error_log("Image creation failed! The memory allocation output (" + force_string(image_memory) + ") is not valid!");
    }

    // Bind the texture image and its memory.
    vkBindImageMemory(logical_device, image, image_memory, 0);

    return { image, image_memory };
}
