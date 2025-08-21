#include <vulkan/vulkan.h>
#include <vector>

#ifndef VULKAN_RENDER_SYNC_FENCES_HPP
#define VULKAN_RENDER_SYNC_FENCES_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

std::vector<VkFence> create_vulkan_fences
(
    const VkDevice &logical_device,
    const uint32_t images_count
);

void destroy_vulkan_fences
(
    const VkDevice &logical_device,
    std::vector<VkFence> &fences
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_Fence
{

public:
    // Constructor.
    Vulkan_Fence
    (
        const VkDevice &logical_device,
        const uint32_t images_count
    );

    // Destructor.
    ~Vulkan_Fence();

    std::vector<VkFence> get() const;

    // Prevent data duplication.
    Vulkan_Fence(const Vulkan_Fence&) = delete;
    Vulkan_Fence &operator = (const Vulkan_Fence&) = delete;

private:
    // We declare the members of the class to store.
    std::vector<VkFence> fences;
    VkDevice logical_device = VK_NULL_HANDLE;

};

#endif
