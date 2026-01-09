#include <vulkan/vulkan.h>

#ifndef VULKAN_TEXTURE_SAMPLER_HPP
#define VULKAN_TEXTURE_SAMPLER_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

VkSampler create_vulkan_texture_sampler
(
    const VkPhysicalDevice &physical_device,
    const VkDevice &logical_device
);

void destroy_vulkan_texture_sampler
(
    const VkDevice &logical_device,
    VkSampler texture_sampler
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_TextureSampler
{

public:
    // Constructor.
    Vulkan_TextureSampler
    (
        const VkPhysicalDevice &physical_device,
        const VkDevice &logical_device
    );

    // Destructor.
    ~Vulkan_TextureSampler();

    VkSampler get() const;

    // Prevent data duplication.
    Vulkan_TextureSampler(const Vulkan_TextureSampler&) = delete;
    Vulkan_TextureSampler &operator = (const Vulkan_TextureSampler&) = delete;

private:
    VkSampler texture_sampler = VK_NULL_HANDLE;
    VkDevice logical_device = VK_NULL_HANDLE;

};

#endif
