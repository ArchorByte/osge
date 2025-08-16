#include <vulkan/vulkan.h>
#include <vector>

#ifndef VULKAN_RENDER_FRAMEBUFFERS_HPP
#define VULKAN_RENDER_FRAMEBUFFERS_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

std::vector<VkFramebuffer> create_vulkan_framebuffers
(
    const VkDevice &logical_device,
    const std::vector<VkImageView> &image_views,
    const VkExtent2D &extent,
    const VkRenderPass &render_pass
);

void destroy_vulkan_framebuffers
(
    const VkDevice &logical_device,
    std::vector<VkFramebuffer> &framebuffers
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_Framebuffers
{

public:
    // Constructor.
    Vulkan_Framebuffers
    (
        const VkDevice &logical_device,
        const std::vector<VkImageView> &image_views,
        const VkExtent2D &extent,
        const VkRenderPass &render_pass
    );

    // Destructor.
    ~Vulkan_Framebuffers();

    std::vector<VkFramebuffer> get() const;

    // Prevent data duplication.
    Vulkan_Framebuffers(const Vulkan_Framebuffers&) = delete;
    Vulkan_Framebuffers &operator = (const Vulkan_Framebuffers&) = delete;

private:
    // We declare the members of the class to store.
    std::vector<VkFramebuffer> framebuffers;
    VkDevice logical_device = VK_NULL_HANDLE;

};

#endif
