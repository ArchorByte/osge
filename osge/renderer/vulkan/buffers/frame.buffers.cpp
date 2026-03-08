#include "vulkan.buffers.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <array>
#include <string>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a frame buffer for each swap chain image view.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Create all frame buffers.

    Parameters:
        - color_image_view / VkImageView         / Describes how to access and treat the color images.
        - depth_image_view / VkImageView         / Describes how to access and treat the depth images.
        - extent           / VkExtent2D          / Resolution of the swap chain.
        - image_views      / vector<VkImageView> / Amount of frame buffers to create. We create one for each swap chain image view.
        - logical_device   / VkDevice            / Logical device of the Vulkan instance.
        - render_pass      / VkRenderPass        / Organizes rendering tasks.

    Returns:
        A vector list containing all created frame buffers.
*/
std::vector<VkFramebuffer> Buffers::create_frame_buffers
(
    const VkImageView              &color_image_view,
    const VkImageView              &depth_image_view,
    const VkExtent2D               &extent,
    const std::vector<VkImageView> &image_views,
    const VkDevice                 &logical_device,
    const VkRenderPass             &render_pass
)
{
    Utils::Logs::log("Creating " + std::to_string(image_views.size()) + " frame buffers.. ", false);

    if (color_image_view == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Color image view invalid.");

    if (depth_image_view == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Depth image view invalid.");

    if (image_views.size() < 1)
        Utils::Logs::crash_log("Failed! No image views provided.");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (render_pass == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Render pass invalid.");

    std::vector<VkFramebuffer> framebuffers;
    framebuffers.reserve(image_views.size());

    for (int i = 0; i < image_views.size(); i++)
    {
        const std::array<VkImageView, 3> attachments = { color_image_view, depth_image_view, image_views[i] };

        /*
            - sType           / Define of the type of the structure.
            - renderPass      / Define which render pass the frame buffer will be working with.
            - attachmentCount / Amount of attachments to pass.
            - pAttachments    / Pass the attachments to use. In this case, we pass the color image view, depth image view and all swap chain image views as attachments.
            - width           / Define the width of the frame buffer. Here, we use the swap chain resolution.
            - height          / Define the height of the frame buffer.
            - layers          / Depth of the frame buffer.
        */
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
            Utils::Logs::crash_log("Failed! Creation #" + std::to_string(i + 1) + " returned error code -> " + std::to_string(framebuffer_creation));

        framebuffers.emplace_back(framebuffer);
    }

    Utils::Logs::log("Done!", true);
    return framebuffers;
}



/*
    Destroy some frame buffers.

    Tasks:
        1) Verify function parameters.
        2) Destroy all valid frame buffers.
        3) Set objects to null.

    Parameters:
        - frame_buffers  / vector<VkFramebuffer> / Frame buffers to destroy.
        - logical_device / VkDevice              / Logical device of the Vulkan instance.

    Returns:
        No object returned.
*/
void Buffers::destroy_frame_buffers
(
    std::vector<VkFramebuffer> &frame_buffers,
    const VkDevice             &logical_device
)
{
    Utils::Logs::log("Destroying " + std::to_string(frame_buffers.size()) + " frame buffers.. ", false);

    if (frame_buffers.size() < 1)
    {
        Utils::Logs::log("Done!", true);
        return;
    }

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    int i = 0;
    int failed = 0;

    for (VkFramebuffer &frame_buffer : frame_buffers)
    {
        i++;

        if (frame_buffer == VK_NULL_HANDLE)
        {
            failed++;
            continue;
        }

        vkDestroyFramebuffer(logical_device, frame_buffer, nullptr);
        frame_buffer = VK_NULL_HANDLE;
    }

    if (failed > 0)
        Utils::Logs::log("Done! Warning: " + std::to_string(failed) + " destructions failed.", true);
    else
        Utils::Logs::log("Done!", true);

    frame_buffers.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Buffers::frame_buffers_handler::frame_buffers_handler
(
    const VkImageView              &color_image_view,
    const VkImageView              &depth_image_view,
    const VkExtent2D               &extent,
    const std::vector<VkImageView> &image_views,
    const VkDevice                 &logical_device,
    const VkRenderPass             &render_pass
)
    : logical_device(logical_device)
{
    frame_buffers = create_frame_buffers(color_image_view, depth_image_view, extent, image_views, logical_device, render_pass);
}

Buffers::frame_buffers_handler::~frame_buffers_handler()
{
    destroy_frame_buffers(frame_buffers, logical_device);
}

std::vector<VkFramebuffer> Buffers::frame_buffers_handler::get() const
{
    return frame_buffers;
}
