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

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Render pass creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    // Reference to the color attachment.
    VkAttachmentReference color_attachment_reference {};
    color_attachment_reference.attachment = 0;                                    // Amount of attachments to pass.
    color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Select the reference layout.

    // Create info for the subpass.
    VkSubpassDescription subpass {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; // Flag to inform that this subpass will be used for a graphics pipeline.
    subpass.colorAttachmentCount = 1;                            // Amount of color attachment to pass.
    subpass.pColorAttachments = &color_attachment_reference;     // Pass the color attachment.

    // Create info for the render pass.
    VkRenderPassCreateInfo pass_info {};
    pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    pass_info.attachmentCount = 1;              // Amount of color attachment to pass.
    pass_info.pAttachments = &color_attachment; // Pass the color attachment.
    pass_info.subpassCount = 1;                 // Amount of subpass to pass.
    pass_info.pSubpasses = &subpass;            // Pass the subpass.

    VkRenderPass render_pass = VK_NULL_HANDLE;
    VkResult pass_creation = vkCreateRenderPass(logical_device, &pass_info, nullptr, &render_pass); // Try to create the render pass.

    if (pass_creation != VK_SUCCESS)
    {
        fatal_error_log("Render pass creation returned error code " + std::to_string(pass_creation) + ".");
    }

    if (!render_pass || render_pass == VK_NULL_HANDLE)
    {
        fatal_error_log("Render pass creation output \"" + force_string(render_pass) + "\" is not valid!");
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

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        error_log("Render pass destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (!render_pass || render_pass == VK_NULL_HANDLE)
    {
        error_log("Render pass destruction failed! The render pass provided (" + force_string(render_pass) + ") is not valid!");
    }

    // Destroy the render pass and dispose of the addresses.
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
