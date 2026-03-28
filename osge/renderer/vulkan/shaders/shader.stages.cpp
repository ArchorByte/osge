#include "vulkan.shaders.hpp"

#include "../../../osge/utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"

#include <map>
#include <vector>

/*
    Create a shader stage for each shader module.
    Warning: Any invalid shader module won't be loaded into the graphics pipeline!

    Tasks:
        1) Verify function parameters.
        2) Create a shader stage for each valid shader module.

    Parameters:
        - shader_modules / vector<ShaderInfo> / Targeted shader modules for the stages creation.

    Returns:
        A vector list containing all shader stages.
*/
std::vector<VkPipelineShaderStageCreateInfo> Shaders::create_shader_stages
(
    const std::vector<ShaderInfo> &shader_modules
)
{
    Utils::Logs::log("Creating " + std::to_string(shader_modules.size()) + " shader stages.. ", false);

    if (shader_modules.size() < 1)
        Utils::Logs::crash_log("Failed! No shaders modules provided.");

    std::vector<VkPipelineShaderStageCreateInfo> shader_stages;
    shader_stages.reserve(shader_modules.size());

    for (const ShaderInfo &shader : shader_modules)
    {
        const std::string shader_type = shader.shader_type;
        const VkShaderModule shader_module = shader.shader_module;

        if (shader_module == VK_NULL_HANDLE)
            continue;

        /*
            - sType  / Defines the type of the structure.
            - stage  / Defines the shader type.
            - module / Passes the shader module.
            - pName  / Defines the entry point name of the shader for this stage.
        */
        const VkPipelineShaderStageCreateInfo create_info
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = shader_type == "vert" ? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = shader_module,
            .pName = "main"
        };

        shader_stages.emplace_back(create_info);
    }

    Utils::Logs::log("Done! " + std::to_string(shader_stages.size()) + " shader stages created.", true);
    return shader_stages;
}
