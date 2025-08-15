#include "framebuffers.handler.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a frame buffer for each swap chain image view.
std::vector<VkFramebuffer> create_vulkan_framebuffers
(
    const VkDevice &logical_device,
    const std::vector<VkImageView> &image_views,
    const VkExtent2D &extent,
    const VkRenderPass &render_pass
)
{
    log("Creating " + std::to_string(image_views.size()) + " frame buffers..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Frame buffers creation failed! The logical device provided (" + force_string(render_pass) + ") is not valid!");
    }

    if (image_views.size() < 1)
    {
        fatal_error_log("Frame buffers creation failed! No image views were provided!");
    }

    if (!render_pass || render_pass == VK_NULL_HANDLE)
    {
        fatal_error_log("Frame buffers creation failed! The render pass provided (" + force_string(render_pass) + ") is not valid!");
    }

    std::vector<VkFramebuffer> framebuffers;

    // Create a frame buffer for each image view.
    for (int i = 0; i < image_views.size(); i++)
    {
        // Select the image view corresponding to the frame buffer index.
        VkImageView image_view = image_views[i];

        // Create info for the frame buffer.
        VkFramebufferCreateInfo framebuffer_create_info {};
        framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_create_info.renderPass = render_pass;   // Pass the render pass.
        framebuffer_create_info.attachmentCount = 1;        // Amount of attachments to pass
        framebuffer_create_info.pAttachments = &image_view; // Pass the swapchain image view as an attachment.
        framebuffer_create_info.width = extent.width;       // Select the width of the frame buffer.
        framebuffer_create_info.height = extent.height;     // Select the height of the frame buffer.
        framebuffer_create_info.layers = 1;                 // Amount of layers to enable.

        VkFramebuffer framebuffer = VK_NULL_HANDLE;
        VkResult framebuffer_creation = vkCreateFramebuffer(logical_device, &framebuffer_create_info, nullptr, &framebuffer);  // Try to create the frame buffer.

        if (framebuffer_creation != VK_SUCCESS)
        {
            fatal_error_log("Frame buffer #" + std::to_string(i + 1) + "/" + std::to_string(framebuffers.size()) + " creation returned error code " + std::to_string(framebuffer_creation));
        }

        if (!framebuffer || framebuffer == VK_NULL_HANDLE)
        {
            fatal_error_log("Frame buffer #" + std::to_string(i + 1) + "/" + std::to_string(framebuffers.size()) + " creation output \"" + force_string(framebuffer) + "\" is not valid!");
        }

        framebuffers.emplace_back(framebuffer); // Register the frame buffer.
        log("- Frame buffer #" + std::to_string(i + 1) + "/" + std::to_string(image_views.size()) + " (" + force_string(framebuffer) + ") created successfully.");
    }

    log(std::to_string(framebuffers.size()) + " frame buffers created successfully!");
    return framebuffers;
}

// Destroy some frame buffers.
void destroy_framebuffers
(
    const VkDevice &logical_device,
    std::vector<VkFramebuffer> &framebuffers
)
{
    log("Destroying " + std::to_string(framebuffers.size()) + " frame buffers..");

    if (!logical_device || logical_device == VK_NULL_HANDLE)
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

    // Destroy each frame buffer in the list.
    for (VkFramebuffer &framebuffer : framebuffers)
    {
        if (!framebuffer || framebuffer == VK_NULL_HANDLE)
        {
            error_log("- Warning: Failed to destroy the frame buffer #" + std::to_string(i + 1) + "/" + std::to_string(framebuffers.size()) + "! The frame buffer provided (" + force_string(framebuffer) + ") is not valid!");
            failed++;
            continue;
        }

        // Destroy the frame buffer and dispose of the address.
        vkDestroyFramebuffer(logical_device, framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;

        log("- Frame buffer #" + std::to_string(i + 1) + "/" + std::to_string(framebuffers.size()) + " destroyed successfully!");
        i++;
    }

    if (failed > 0) error_log("Warning: " + std::to_string(failed) + " frame buffers failed to destroy! This might leads to some memory leaks!");
    log(std::to_string(framebuffers.size() - failed) + "/" + std::to_string(framebuffers.size()) + " frame buffers destroyed successfully!");

    // Free the list.
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
    const VkExtent2D &extent,
    const VkRenderPass &render_pass
) : logical_device(logical_device)
{
    framebuffers = create_vulkan_framebuffers(logical_device, image_views, extent, render_pass);
}

// Destructor.
Vulkan_Framebuffers::~Vulkan_Framebuffers()
{
    destroy_framebuffers(logical_device, framebuffers);
}

std::vector<VkFramebuffer> Vulkan_Framebuffers::get() const
{
    return framebuffers;
}
