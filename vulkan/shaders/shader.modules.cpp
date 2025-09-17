#include "shader.modules.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.files.hpp"
#include "../../helpers/help.text.format.hpp"

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
    std::string &file_name
)
{
    if (!logical_device || logical_device == VK_NULL_HANDLE)
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

    // Create info for the shader module.
    VkShaderModuleCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = binaries.size();                                 // Size of the data we are going to pass as binaries.
    info.pCode = reinterpret_cast<const uint32_t*>(binaries.data()); // We pass the binaries.

    VkShaderModule shader_module = VK_NULL_HANDLE;
    VkResult module_creation = vkCreateShaderModule(logical_device, &info, nullptr, &shader_module); // Try to create the module.

    if (module_creation != VK_SUCCESS)
    {
        fatal_error_log("- Shader module \"" + file_name + "\" creation returned error code " + std::to_string(module_creation) + ".");
    }

    if (!shader_module || shader_module == VK_NULL_HANDLE)
    {
        fatal_error_log("- Shader module \"" + file_name + "\" creation output \"" + force_string(shader_module) + "\" is not valid!");
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
    std::vector<ShaderInfo> shaders_modules;

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        fatal_error_log("Shaders modules creation failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
    }

    int total = 0;
    int failed = 0;

    // Read each file in the shaders folder.
    for (const auto &file : fs::directory_iterator("./shaders"))
    {
        total++;
        std::string file_name = file.path().filename().string();

        if (!fs::is_regular_file(file.status()))
        {
            error_log("- Warning: The creation of the shader module \"" + file_name + "\" failed! It's not a valid file!");
            continue;
        }

        if (file.path().extension() != ".frag" && file.path().extension() != ".vert")
        {
            error_log("- Warning: The creation of the shader module \"" + file_name + "\" failed! It's not a Vertex or a Fragment shader!");
            continue;
        }

        std::string type = file.path().extension() == ".vert" ? "vert" : "frag";                                // Determine if it's a Vertex or a Fragment shader.
        std::vector<char> shader_binaries = read_binary_file("./shaders/" + file_name);                         // Read the shader file binaries.
        VkShaderModule shader_module = create_vulkan_shader_module(logical_device, shader_binaries, file_name); // Create the shader module.

        ShaderInfo shader_info
        {
            type,         // Vertex ("vert") or Fragment ("frag") shader.
            shader_module // The actual shader module.
        };

        shaders_modules.emplace_back(shader_info); // Register the shader module in the list.
        log("- Shader module \"" + file_name + "\" (" + force_string(shader_module) + ") created successfully!");
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

    if (!logical_device || logical_device == VK_NULL_HANDLE)
    {
        error_log("Shaders modules destruction failed! The logical device provided (" + force_string(logical_device) + ") is not valid!");
        return;
    }

    if (shaders_modules.size() < 1)
    {
        error_log("Shaders modules destruction failed! No shaders modules were provided!");
        return;
    }

    int failed = 0;
    int i = 0;

    // Destroy each shader module in the list.
    for (ShaderInfo &data : shaders_modules)
    {
        i++;
        VkShaderModule shader_module = data.shader_module; // Get the shader module in the ShaderInfo structure.

        if (!shader_module || shader_module == VK_NULL_HANDLE)
        {
            error_log("- Warning: Failed to destroy the shader module #" + std::to_string(i) + "/" + std::to_string(shaders_modules.size()) + " destruction failed! The shader module provided (" + force_string(shader_module) + ") is not valid!");
            failed++;
            continue;
        }

        // Destroy the shader module and dispose of the address.
        vkDestroyShaderModule(logical_device, shader_module, nullptr);
        shader_module = VK_NULL_HANDLE;

        log("- Shader module #" + std::to_string(i) + "/" + std::to_string(shaders_modules.size()) + " destroyed successfully!");
    }

    if (failed > 0) error_log("Warning: " + std::to_string(failed) + " shaders modules failed to destroy! This might lead to some memory leaks!");
    log(std::to_string(shaders_modules.size() - failed) + "/" + std::to_string(shaders_modules.size()) + " shaders modules destroyed successfully!");

    // Free the list.
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
