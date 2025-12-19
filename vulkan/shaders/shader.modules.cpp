#include "shader.modules.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.files.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <map>
#include <vector>
#include <cstdint>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create one specific shader module using a binary file.
// Note: This function only logs errors, as it was meant to be only used by the engine.
VkShaderModule create_vulkan_shader_module
(
    const VkDevice &logical_device,
    const std::vector<char> &binaries,
    const std::string &file_name
)
{
    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Shader module \"" + file_name + "\" creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    if (binaries.size() < 1)
    {
        fatal_error_log("Shader module \"" + file_name + "\" creation failed! No binaries were provided!");
    }

    if (trim(file_name).size() < 1)
    {
        fatal_error_log("Shader module \"" + file_name + "\" creation failed! The file name provided is not valid!");
    }

    const VkShaderModuleCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = binaries.size(),
        .pCode = reinterpret_cast<const uint32_t*>(binaries.data()) // We pass the shader's binaries.
    };

    VkShaderModule shader_module = VK_NULL_HANDLE;
    const VkResult module_creation = vkCreateShaderModule(logical_device, &create_info, nullptr, &shader_module);

    if (module_creation != VK_SUCCESS)
    {
        fatal_error_log("- Shader module \"" + file_name + "\" creation returned error code " + std::to_string(module_creation) + ".");
    }

    if (shader_module == VK_NULL_HANDLE)
    {
        fatal_error_log("- Shader module \"" + file_name + "\" creation output (" + force_string(shader_module) + ") is not valid!");
    }

    return shader_module;
}

// Create all shader modules at once in the shaders directory.
// Note: We only trigger a crash if we started the shader module creation process. If a file is not valid, we just ignore it.
std::vector<ShaderInfo> create_all_vulkan_shader_modules
(
    const VkDevice &logical_device
)
{
    log("Creating the shaders modules..");

    if (logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Shaders modules creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    int total = 0;
    int failed = 0;

    std::vector<ShaderInfo> shaders_modules;

    for (const auto &file : fs::directory_iterator("./shaders"))
    {
        total++;

        const std::string file_name = file.path().filename().string();
        const std::string file_extension = file.path().extension().string();

        if (!fs::is_regular_file(file.status()))
        {
            error_log("- The creation of the shader module \"" + file_name + "\" failed! It's not a valid file!");
            continue;
        }

        if (file_extension != ".frag" && file_extension != ".vert")
        {
            error_log("- The creation of the shader module \"" + file_name + "\" failed! It's neither a vertex nor a fragment shader!");
            continue;
        }

        const std::string type = file_extension == ".vert" ? "vert" : "frag";
        const std::vector<char> shader_binaries = read_binary_file("./shaders/" + file_name);
        const VkShaderModule shader_module = create_vulkan_shader_module(logical_device, shader_binaries, file_name);

        const ShaderInfo shader_info
        {
            type,
            shader_module
        };

        shaders_modules.emplace_back(shader_info);
        log("- Shader module \"" + file_name + "\" (" + force_string(shader_module) + ") created successfully!");
    }

    if (failed != 0)
    {
        error_log("Warning: " + std::to_string(failed) + " shader modules creation failed!");
    }

    log(std::to_string(shaders_modules.size()) + "/" + std::to_string(total) + " shaders modules created successfully!");
    return shaders_modules;
}

// Destroy some shader modules.
void destroy_vulkan_shader_modules
(
    const VkDevice &logical_device,
    std::vector<ShaderInfo> &shaders_modules
)
{
    log("Destroying " + std::to_string(shaders_modules.size()) + " shaders modules..");

    if (logical_device == VK_NULL_HANDLE)
    {
        error_log("Shader modules destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (shaders_modules.size() < 1)
    {
        error_log("Shader modules destruction failed! No shader modules were provided!");
        return;
    }

    int failed = 0;
    int i = 0;

    for (ShaderInfo &data : shaders_modules)
    {
        i++;
        VkShaderModule shader_module = data.shader_module;

        if (shader_module == VK_NULL_HANDLE)
        {
            error_log("- Failed to destroy the shader module #" + std::to_string(i) + "/" + std::to_string(shaders_modules.size()) + "! The shader module provided (" + force_string(shader_module) + ") is not valid!");
            failed++;
            continue;
        }

        vkDestroyShaderModule(logical_device, shader_module, nullptr);
        shader_module = VK_NULL_HANDLE;

        log("- Shader module #" + std::to_string(i) + "/" + std::to_string(shaders_modules.size()) + " destroyed successfully!");
    }

    if (failed != 0)
    {
        error_log("Warning: " + std::to_string(failed) + " shader modules failed to destroy! This might lead to some memory leaks!");
    }

    log(std::to_string(shaders_modules.size() - failed) + "/" + std::to_string(shaders_modules.size()) + " shader modules destroyed successfully!");
    shaders_modules.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_ShadersModules::Vulkan_ShadersModules
(
    const VkDevice &logical_device
) : logical_device(logical_device)
{
    shaders_modules = create_all_vulkan_shader_modules(logical_device);
}

// Destructor.
Vulkan_ShadersModules::~Vulkan_ShadersModules()
{
    destroy_vulkan_shader_modules(logical_device, shaders_modules);
}

std::vector<ShaderInfo> Vulkan_ShadersModules::get() const
{
    return shaders_modules;
}
