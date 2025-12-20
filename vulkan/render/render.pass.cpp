#include "render.pass.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <array>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a render pass.
VkRenderPass create_vulkan_render_pass
(
    const VkDevice &logical_device,
    const VkAttachmentDescription &color_attachment,
    const VkAttachmentDescription &depth_attachment,
    const VkAttachmentReference &depth_attachment_reference,
    const VkSurfaceFormatKHR &surface_format
)
{
    log("Creating a render pass..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Render pass creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    VkAttachmentDescription resolve
    {
        .format = surface_format.format,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };

    const VkAttachmentReference resolve_attachment_reference
    {
        .attachment = 2,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    const VkAttachmentReference color_attachment_reference
    {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL // Select the reference layout.
    };

    const VkSubpassDescription subpass
    {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS, // This subpass will be used for a graphics pipeline.
        .colorAttachmentCount = 1,
        .pColorAttachments = &color_attachment_reference,
        .pResolveAttachments = &resolve_attachment_reference,
        .pDepthStencilAttachment = &depth_attachment_reference
    };

    const VkSubpassDependency dependency
    {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
    };

    const std::array<VkAttachmentDescription, 3> attachments = { color_attachment, depth_attachment, resolve };

    const VkRenderPassCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = static_cast<uint32_t>(attachments.size()),
        .pAttachments = attachments.data(),
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &dependency
    };

    VkRenderPass render_pass = VK_NULL_HANDLE;
    const VkResult pass_creation = vkCreateRenderPass(logical_device, &create_info, nullptr, &render_pass);

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
    const VkAttachmentDescription &color_attachment,
    const VkAttachmentDescription &depth_attachment,
    const VkAttachmentReference &depth_attachment_reference,
    const VkSurfaceFormatKHR &surface_format
) : logical_device(logical_device)
{
    render_pass = create_vulkan_render_pass(logical_device, color_attachment, depth_attachment, depth_attachment_reference, surface_format);
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
