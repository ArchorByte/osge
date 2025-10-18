#include "render.pass.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a render pass.
VkRenderPass create_vulkan_render_pass
(
    const VkDevice &logical_device,
    const VkAttachmentDescription &color_attachment
)
{
    log("Creating a render pass..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Render pass creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    VkAttachmentReference attachment_reference
    {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL // Select the reference layout.
    };

    VkSubpassDescription subpass
    {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS, // This subpass will be used for a graphics pipeline.
        .colorAttachmentCount = 1,
        .pColorAttachments = &attachment_reference
    };

    VkRenderPassCreateInfo pass_create_info
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 1,
        .pAttachments = &color_attachment,
        .subpassCount = 1,
        .pSubpasses = &subpass
    };

    VkRenderPass render_pass = VK_NULL_HANDLE;
    const VkResult pass_creation = vkCreateRenderPass(logical_device, &pass_create_info, nullptr, &render_pass);

    if (pass_creation != VK_SUCCESS)
    {
        fatal_error_log("Render pass creation returned error code " + std::to_string(pass_creation) + ".");
    }

    if (render_pass == VK_NULL_HANDLE)
    {
        fatal_error_log("Render pass creation output (" + force_string(render_pass) + ") is not valid!");
    }

    log("Render pass " + force_string(render_pass) + " created successfully!");
    return render_pass;
}

// Destroy a render pass.
void destroy_vulkan_render_pass
(
    const VkDevice &logical_device,
    VkRenderPass &render_pass
)
{
    log("Destroying the " + force_string(render_pass) + " render pass..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Render pass destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (render_pass == VK_NULL_HANDLE)
    {
        error_log("Render pass destruction failed! The render pass provided (" + force_string(render_pass) + ") is not valid!");
    }

    vkDestroyRenderPass(logical_device, render_pass, nullptr);
    render_pass = VK_NULL_HANDLE;

    log("Render pass destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_RenderPass::Vulkan_RenderPass
(
    const VkDevice &logical_device,
    const VkAttachmentDescription &color_attachment
) : logical_device(logical_device)
{
    render_pass = create_vulkan_render_pass(logical_device, color_attachment);
}

// Destructor.
Vulkan_RenderPass::~Vulkan_RenderPass()
{
    destroy_vulkan_render_pass(logical_device, render_pass);
}

VkRenderPass Vulkan_RenderPass::get() const
{
    return render_pass;
}
