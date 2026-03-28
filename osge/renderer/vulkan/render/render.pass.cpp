#include "vulkan.render.hpp"

#include "../../../utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"

#include <array>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a render pass.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Make attachment references.
        3) Make subpass.
        4) Create render pass.

    Parameters:
        - color_attachment / VkAttachmentDescription / Describes color attachment for rendering output.
        - depth_attachment / VkAttachmentDescription / Describes depth/stencil attachment.
        - logical_device   / VkDevice                / Logical device of the Vulkan instance.
        - surface_format   / VkSurfaceFormatKHR      / Format of swap chain images.

    Returns:
        The created render pass.
*/
VkRenderPass Render::create_render_pass
(
    const VkAttachmentDescription &color_attachment,
    const VkAttachmentDescription &depth_attachment,
    const VkDevice                &logical_device,
    const VkSurfaceFormatKHR      &surface_format
)
{
    Utils::Logs::log("Creating render pass.. ", false);

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    /*
        - format         / Defines the format of the image view that will be used for the attachment.
        - samples        / Amount of samples of the image.
        - loadOp         / Defines how the content of the attachment will be used by the subpass. Here, we forces the render area to be cleared.
        - storeOp        / Defines what happens to the content of the attachment once the subpass ended. Here, we just ignore the content.
        - stencilLoadOp  / Defines how the content of the stencil components will be used by the subpass. Here, we just ignore the previous data and overwrite it.
        - stencilStoreOp / Defines what happens to the content of the stencil components once the subpass ended. Here, we just ignore the content.
        - initialLayout  / Defines the initial layout of the attachment.
        - finalLayout    / Defines the layout to transition to once the render pass ended.
    */
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

    /*
        - attachment / Integer identifying the attachment.
        - layout     / Defines which layout is used for the attachment.
    */
    const VkAttachmentReference color_attachment_reference
    {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    VkAttachmentReference depth_attachment_reference
    {
        .attachment = 1,
        .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };

    const VkAttachmentReference resolve_attachment_reference
    {
        .attachment = 2,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    /*
        - pipelineBindPoint       / Defines which pipeline type is supported by this subpass.
        - colorAttachmentCount    / Amount of color attachments to pass.
        - pColorAttachments       / Passes color attachments.
        - pResolveAttachments     / Passes attachment references.
        - pDepthStencilAttachment / Passes depth stencil attachments.
    */
    const VkSubpassDescription subpass
    {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &color_attachment_reference,
        .pResolveAttachments = &resolve_attachment_reference,
        .pDepthStencilAttachment = &depth_attachment_reference
    };

    /*
        - srcSubpass    / Source subpass we make data from.
        - dstSubpass    / Destination subpass.
        - srcStageMask  / Pipeline stages to wait on .
        - dstStageMask  / Pipeline stages that wait for the dependency.
        - srcAccessMask / Access mask that must be completed before continuing.
        - dstAccessMask / Access mask that wait on the dependency.
    */
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

    /*
        - sType           / Defines the type of the structure.
        - attachmentCount / Amount of attachments to pass.
        - pAttachments    / Passes the attachments.
        - subpassCount    / Amount of subpasses to pass.
        - pSubpasses      / Passes the subpasses.
        - dependencyCount / Amount of dependencies to pass.
        - pDependencies   / Passes the dependencies.
    */
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
        Utils::Logs::crash_log("Failed! Creation returned error code -> " + std::to_string(pass_creation) + ".");

    Utils::Logs::log("Done! Memory address -> " + Utils::Text::get_memory_address(render_pass) + ".", true);
    return render_pass;
}



/*
    Destroy a render pass.

    Tasks:
        1) Verify function parameters.
        2) Destroy render pass.
        3) Set object to null.

    Parameters:
        - logical_device / VkDevice     / Logical device of the Vulkan instance.
        - render_pass    / VkRenderPass / Render pass to destroy.

    Returns:
        No object returned.
*/
void Render::destroy_render_pass
(
    const VkDevice &logical_device,
    VkRenderPass   &render_pass
)
{
    Utils::Logs::log("Destroying " + Utils::Text::get_memory_address(render_pass) + " render pass.. ", false);

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    if (render_pass == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Render pass invalid.", true);
        return;
    }

    vkDestroyRenderPass(logical_device, render_pass, nullptr);
    render_pass = VK_NULL_HANDLE;

    Utils::Logs::log("Done!", true);
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Render::render_pass_handler::render_pass_handler
(
    const VkAttachmentDescription &color_attachment,
    const VkAttachmentDescription &depth_attachment,
    const VkDevice                &logical_device,
    const VkSurfaceFormatKHR      &surface_format
)
    : logical_device(logical_device)
{
    render_pass = create_render_pass(color_attachment, depth_attachment, logical_device, surface_format);
}

Render::render_pass_handler::~render_pass_handler()
{
    destroy_render_pass(logical_device, render_pass);
}

VkRenderPass Render::render_pass_handler::get() const
{
    return render_pass;
}
