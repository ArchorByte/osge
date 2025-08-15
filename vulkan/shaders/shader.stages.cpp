#include "shader.stages.hpp"

#include "shader.modules.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <map>

// Create a shader stage for each shader module for a graphics pipeline.
// Note: Every invalid shader module won't have a shader stage and so won't be loaded to the graphics pipeline!
std::vector<VkPipelineShaderStageCreateInfo> create_vulkan_shaders_stages
(
    const std::vector<ShaderInfo> &shaders_modules
)
{
    log("Creating " + std::to_string(shaders_modules.size()) + " shaders stages..");

    if (shaders_modules.size() < 1)
    {
        fatal_error_log("Shaders stages creation failed! No shaders modules were provided!");
    }

    std::vector<VkPipelineShaderStageCreateInfo> shaders_stages;
    int i = 0;

    // Create a shader stage for each shader module in the list.
    for (const ShaderInfo &shader : shaders_modules)
    {
        i++;

        // Read the ShaderInfo structure to retrieve the shader module data.
        std::string shader_type = shader.shader_type;
        VkShaderModule shader_module = shader.shader_module;

        if (!shader_module || shader_module == VK_NULL_HANDLE)
        {
            error_log("- Warning: Failed to create the shader stage #" + std::to_string(i) + "/" + std::to_string(shaders_modules.size()) + "! The shader module provided (" + force_string(shader_module) + ") is not valid!");
            continue;
        }

        // Create info for the shader stage.
        VkPipelineShaderStageCreateInfo info {};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        info.stage = shader_type == "vert" ? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT; // Select between Vertex or Fragment flag.
        info.module = shader_module;                                                                    // Pass the shader module.
        info.pName = "main";

        shaders_stages.emplace_back(info); // Register the shader stage.
        log("- Shader stage #" + std::to_string(i) + "/" + std::to_string(shaders_stages.size()) + " created successfully!");
    }

    log(std::to_string(shaders_stages.size()) + "/" + std::to_string(shaders_modules.size()) + " shaders stages created successfully!");
    return shaders_stages;
}
