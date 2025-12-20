#include "render.framebuffers.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <array>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a frame buffer for each swap chain image view.
std::vector<VkFramebuffer> create_vulkan_framebuffers
(
    const VkDevice &logical_device,
    const std::vector<VkImageView> &image_views,
    const VkImageView &color_image_view,
    const VkImageView &depth_image_view,
    const VkExtent2D &extent,
    const VkRenderPass &render_pass
)
{
    log("Creating " + std::to_string(image_views.size()) + " frame buffers..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Frame buffers creation failed! The logical device provided (" + force_string(render_pass) + ") is not valid!");
    }

    if (image_views.size() < 1)
    {
        fatal_error_log("Frame buffers creation failed! No image views were provided!");
    }

    if (depth_image_view == VK_NULL_HANDLE)
    {
        fatal_error_log("Frame buffers creation failed! The depth image view provided (" + force_string(depth_image_view) + ") is not valid!");
    }

    if (render_pass == VK_NULL_HANDLE)
    {
        fatal_error_log("Frame buffers creation failed! The render pass provided (" + force_string(render_pass) + ") is not valid!");
    }

    std::vector<VkFramebuffer> framebuffers;
    framebuffers.reserve(image_views.size());

    for (int i = 0; i < image_views.size(); i++)
    {
        const std::array<VkImageView, 3> attachments = { color_image_view, depth_image_view, image_views[i] };

        const VkFramebufferCreateInfo create_info
        {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = render_pass,
            .attachmentCount = static_cast<uint32_t>(attachments.size()),
            .pAttachments = attachments.data(),
            .width = extent.width,
            .height = extent.height,
            .layers = 1 // Amount of layers to enable.
        };

        VkFramebuffer framebuffer = VK_NULL_HANDLE;
        const VkResult framebuffer_creation = vkCreateFramebuffer(logical_device, &create_info, nullptr, &framebuffer);

        if (framebuffer_creation != VK_SUCCESS)
        {
            fatal_error_log("Frame buffer #" + std::to_string(i + 1) + "/" + std::to_string(framebuffers.size()) + " creation returned error code " + std::to_string(framebuffer_creation));
        }

        if (framebuffer == VK_NULL_HANDLE)
        {
            fatal_error_log("Frame buffer #" + std::to_string(i + 1) + "/" + std::to_string(framebuffers.size()) + " creation output (" + force_string(framebuffer) + ") is not valid!");
        }

        framebuffers.emplace_back(framebuffer);
        log("- Frame buffer #" + std::to_string(i + 1) + "/" + std::to_string(image_views.size()) + " (" + force_string(framebuffer) + ") created successfully.");
    }

    log(std::to_string(framebuffers.size()) + " frame buffers created successfully!");
    return framebuffers;
}

// Destroy some frame buffers.
void destroy_vulkan_framebuffers
(
    const VkDevice &logical_device,
    std::vector<VkFramebuffer> &framebuffers
)
{
    log("Destroying " + std::to_string(framebuffers.size()) + " frame buffers..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Frame buffers destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (framebuffers.size() < 1)
    {
        error_log("Frame buffers destruction failed! No frame buffers were provided!");
        return;
    }

    int i = 0;
    int failed = 0;

    for (VkFramebuffer &framebuffer : framebuffers)
    {
        i++;

        if (framebuffer == VK_NULL_HANDLE)
        {
            error_log("- Failed to destroy the frame buffer #" + std::to_string(i) + "/" + std::to_string(framebuffers.size()) + "! The frame buffer provided (" + force_string(framebuffer) + ") is not valid!");
            failed++;
            continue;
        }

        vkDestroyFramebuffer(logical_device, framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;

        log("- Frame buffer #" + std::to_string(i) + "/" + std::to_string(framebuffers.size()) + " destroyed successfully!");
    }

    if (failed > 0)
    {
        error_log("Warning: " + std::to_string(failed) + " frame buffers failed to destroy! This might leads to some memory leaks!");
    }

    log(std::to_string(framebuffers.size() - failed) + "/" + std::to_string(framebuffers.size()) + " frame buffers destroyed successfully!");
    framebuffers.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_Framebuffers::Vulkan_Framebuffers
(
    const VkDevice &logical_device,
    const std::vector<VkImageView> &image_views,
    const VkImageView &color_image_view,
    const VkImageView &depth_image_view,
    const VkExtent2D &extent,
    const VkRenderPass &render_pass
) : logical_device(logical_device)
{
    framebuffers = create_vulkan_framebuffers(logical_device, image_views, color_image_view, depth_image_view, extent, render_pass);
}

// Destructor.
Vulkan_Framebuffers::~Vulkan_Framebuffers()
{
    destroy_vulkan_framebuffers(logical_device, framebuffers);
}

std::vector<VkFramebuffer> Vulkan_Framebuffers::get() const
{
    return framebuffers;
}
