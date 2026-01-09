#include "vulkan.buffers.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <array>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a frame buffer for each swap chain image view.
    Note: You should use the pre-made class to handle the frame buffers rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the parameters.
        2) Create all frame buffers.

    Parameters:
        - color_image_view / VkImageView         / View of the color image.
        - depth_image_view / VkImageView         / View of the depth image.
        - extent           / VkExtent2D          / Resolution of the swap chain.
        - image_views      / vector<VkImageView> / Targeted swap chain image views.
        - logical_device   / VkDevice            / Logical device of the Vulkan instance.
        - render_pass      / VkRenderPass        / Render pass of the Vulkan instance.

    Returns:
        A vector list containing all created frame buffers.
*/
std::vector<VkFramebuffer> Vulkan::Buffers::create_frame_buffers
(
    const VkImageView &color_image_view,
    const VkImageView &depth_image_view,
    const VkExtent2D &extent,
    const std::vector<VkImageView> &image_views,
    const VkDevice &logical_device,
    const VkRenderPass &render_pass
)
{
    log("Creating " + std::to_string(image_views.size()) + " frame buffers..");

    if (color_image_view == VK_NULL_HANDLE)
        fatal_error_log("Frame buffers creation failed! The color image view provided (" + force_string(color_image_view) + ") is not valid!");

    if (depth_image_view == VK_NULL_HANDLE)
        fatal_error_log("Frame buffers creation failed! The depth image view provided (" + force_string(depth_image_view) + ") is not valid!");

    if (image_views.size() < 1)
        fatal_error_log("Frame buffers creation failed! No image views provided!");

    if (logical_device == VK_NULL_HANDLE)
        fatal_error_log("Frame buffers creation failed! The logical device provided (" + force_string(render_pass) + ") is not valid!");

    if (render_pass == VK_NULL_HANDLE)
        fatal_error_log("Frame buffers creation failed! The render pass provided (" + force_string(render_pass) + ") is not valid!");

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
            .layers = 1
        };

        VkFramebuffer framebuffer = VK_NULL_HANDLE;
        const VkResult framebuffer_creation = vkCreateFramebuffer(logical_device, &create_info, nullptr, &framebuffer);

        if (framebuffer_creation != VK_SUCCESS)
            fatal_error_log("Frame buffer #" + std::to_string(i + 1) + "/" + std::to_string(framebuffers.size()) + " creation returned error code " + std::to_string(framebuffer_creation));

        framebuffers.emplace_back(framebuffer);
        log("- Frame buffer #" + std::to_string(i + 1) + "/" + std::to_string(image_views.size()) + " (" + force_string(framebuffer) + ") created successfully.");
    }

    log(std::to_string(framebuffers.size()) + " frame buffers created successfully!");
    return framebuffers;
}



/*
    Cleanly destroy all frame buffers.

    Tasks:
        1) Verify the parameters.
        2) Destroy all valid frame buffers.

    Parameters:
        - frame_buffers  / vector<VkFramebuffer> / Frame buffers to destroy.
        - logical_device / VkDevice              / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Vulkan::Buffers::destroy_frame_buffers
(
    std::vector<VkFramebuffer> &frame_buffers,
    const VkDevice &logical_device
)
{
    log("Destroying " + std::to_string(frame_buffers.size()) + " frame buffers..");

    if (frame_buffers.size() < 1)
    {
        error_log("Frame buffers destruction failed! No frame buffers provided!");
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Frame buffers destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    int i = 0;
    int failed = 0;

    for (VkFramebuffer &frame_buffer : frame_buffers)
    {
        i++;

        if (frame_buffer == VK_NULL_HANDLE)
        {
            error_log("- Failed to destroy the frame buffer #" + std::to_string(i) + "/" + std::to_string(frame_buffers.size()) + "! The frame buffer provided (" + force_string(frame_buffer) + ") is not valid!");
            failed++;
            continue;
        }

        vkDestroyFramebuffer(logical_device, frame_buffer, nullptr);
        frame_buffer = VK_NULL_HANDLE;

        log("- Frame buffer #" + std::to_string(i) + "/" + std::to_string(frame_buffers.size()) + " destroyed successfully!");
    }

    if (failed > 0)
        error_log("Warning: " + std::to_string(failed) + " frame buffers failed to destroy! This might leads to some memory leaks!");

    log(std::to_string(frame_buffers.size() - failed) + "/" + std::to_string(frame_buffers.size()) + " frame buffers destroyed successfully!");
    frame_buffers.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Vulkan::Buffers::frame_buffers_handler::frame_buffers_handler
(
    const VkImageView &color_image_view,
    const VkImageView &depth_image_view,
    const VkExtent2D &extent,
    const std::vector<VkImageView> &image_views,
    const VkDevice &logical_device,
    const VkRenderPass &render_pass
)
    : logical_device(logical_device)
{
    frame_buffers = Vulkan::Buffers::create_frame_buffers(color_image_view, depth_image_view, extent, image_views, logical_device, render_pass);
}

Vulkan::Buffers::frame_buffers_handler::~frame_buffers_handler()
{
    Vulkan::Buffers::destroy_frame_buffers(frame_buffers, logical_device);
}

std::vector<VkFramebuffer> Vulkan::Buffers::frame_buffers_handler::get() const
{
    return frame_buffers;
}
