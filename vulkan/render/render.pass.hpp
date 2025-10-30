#include <vulkan/vulkan.h>

#ifndef VULKAN_RENDER_PASS_HPP
#define VULKAN_RENDER_PASS_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

VkRenderPass create_vulkan_render_pass
(
    const VkDevice &logical_device,
    const VkAttachmentDescription &color_attachment,
    const VkAttachmentDescription &depth_attachment,
    const VkAttachmentReference &depth_attachment_reference
);

void destroy_vulkan_render_pass
(
    const VkDevice &logical_device,
    VkRenderPass &render_pass
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_RenderPass
{

public:
    // Constructor.
    Vulkan_RenderPass
    (
        const VkDevice &logical_device,
        const VkAttachmentDescription &color_attachment,
        const VkAttachmentDescription &depth_attachment,
        const VkAttachmentReference &depth_attachment_reference
    );

    // Destructor.
    ~Vulkan_RenderPass();

    VkRenderPass get() const;

    Vulkan_RenderPass(const Vulkan_RenderPass&) = delete;
    Vulkan_RenderPass &operator = (const Vulkan_RenderPass&) = delete;

private:
    // We declare the members of the class to store.
    VkRenderPass render_pass = VK_NULL_HANDLE;
    VkDevice logical_device = VK_NULL_HANDLE;

};

#endif
