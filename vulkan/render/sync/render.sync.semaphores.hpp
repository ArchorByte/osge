#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

#ifndef VULKAN_RENDER_SYNC_SEMAPHORES_HPP
#define VULKAN_RENDER_SYNC_SEMAPHORES_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

std::vector<VkSemaphore> create_vulkan_semaphores
(
    const VkDevice &logical_device,
    const uint32_t &images_count
);

void destroy_semaphores
(
    const VkDevice &logical_device,
    std::vector<VkSemaphore> &semaphores
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_Semaphores
{

public:
    // Constructor.
    Vulkan_Semaphores
    (
        const VkDevice &logical_device,
        const uint32_t &images_count
    );

    // Destructor.
    ~Vulkan_Semaphores();

    std::vector<VkSemaphore> get() const;

    // Prevent data duplication.
    Vulkan_Semaphores(const Vulkan_Semaphores&) = delete;
    Vulkan_Semaphores &operator = (const Vulkan_Semaphores&) = delete;

private:
    // We declare the members of the class to store.
    std::vector<VkSemaphore> semaphores;
    VkDevice logical_device = VK_NULL_HANDLE;

};

#endif
