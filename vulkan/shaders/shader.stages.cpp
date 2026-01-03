#include "vulkan.shaders.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <map>
#include <vector>
#include <vulkan/vulkan.h>

/*
    Create a shader stage for each shader module.
    Warning: Any invalid shader module won't be loaded into the graphics pipeline!

    Tasks:
        1) Verify the parameters.
        2) Create a shader stage for each valid shader module.

    Parameters:
        - shader_modules / vector<ShaderInfo> / Targeted shader modules for the stages creation.

    Returns:
        A vector list containing all shader stages.
*/
std::vector<VkPipelineShaderStageCreateInfo> Vulkan::Shaders::create_shader_stages
(
    const std::vector<ShaderInfo> &shader_modules
)
{
    log("Creating " + std::to_string(shader_modules.size()) + " shader stages..");

    if (shader_modules.size() < 1)
        fatal_error_log("Shader stages creation failed! No shaders modules provided!");

    std::vector<VkPipelineShaderStageCreateInfo> shader_stages;
    shader_stages.reserve(shader_modules.size());
    int i = 0;

    for (const ShaderInfo &shader : shader_modules)
    {
        i++;

        const std::string shader_type = shader.shader_type;
        const VkShaderModule shader_module = shader.shader_module;

        if (shader_module == VK_NULL_HANDLE)
        {
            error_log("- Failed to create the shader stage #" + std::to_string(i) + "/" + std::to_string(shader_modules.size()) + "! The shader module provided (" + force_string(shader_module) + ") is not valid!");
            continue;
        }

        const VkPipelineShaderStageCreateInfo create_info
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = shader_type == "vert" ? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = shader_module,
            .pName = "main"
        };

        shader_stages.emplace_back(create_info);
        log("- Shader stage #" + std::to_string(i) + "/" + std::to_string(shader_stages.size()) + " created successfully!");
    }

    log(std::to_string(shader_stages.size()) + "/" + std::to_string(shader_modules.size()) + " shader stages created successfully!");
    return shader_stages;
}
