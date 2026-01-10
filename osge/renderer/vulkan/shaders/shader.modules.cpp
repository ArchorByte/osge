#include "vulkan.shaders.hpp"
#include "osge/utils/utils.hpp"
#include <cstdint>
#include <filesystem>
#include <libraries/vulkan/vulkan.h>
#include <map>
#include <string>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a shader module.
    Note: This function was meant to be used by the create_all_shader_modules function only, itself meant to be used with the pre-made class.
          If you create shader modules by yourself using this function, you will have to destroy them yourself as well.

    Tasks:
        1) Verify the parameters.
        2) Create the shader module.

    Parameters:
        - file_name       / string          / Name of the shader file.
        - logical_device  / VkDevice        / Logical device of the Vulkan instance.
        - shader_binaries / shader_binaries / Binaries of the shader.

    Returns:
        The created shader module.
*/
VkShaderModule Vulkan::Shaders::create_shader_module
(
    const std::string &file_name,
    const VkDevice &logical_device,
    const std::vector<char> &shader_binaries
)
{
    if (shader_binaries.size() < 1)
        Utils::Logs::crash_error_log("Shader module \"" + file_name + "\" creation failed! No binaries provided!");

    if (Utils::Text::trim(file_name).size() < 1)
        Utils::Logs::crash_error_log("Shader module \"" + file_name + "\" creation failed! The file name provided is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Shader module \"" + file_name + "\" creation failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    const VkShaderModuleCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = shader_binaries.size(),
        .pCode = reinterpret_cast<const uint32_t*>(shader_binaries.data())
    };

    VkShaderModule shader_module = VK_NULL_HANDLE;
    const VkResult module_creation = vkCreateShaderModule(logical_device, &create_info, nullptr, &shader_module);

    if (module_creation != VK_SUCCESS)
        Utils::Logs::crash_error_log("- Shader module \"" + file_name + "\" creation returned error code " + std::to_string(module_creation) + ".");

    return shader_module;
}



/*
    Create a shader module for each valid shader found in the shaders folder.
    Note: You should use the pre-made class to handle the shader modules rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the parameters.
        2) Verify the files validity.
        3) Load the shaders (create the modules) and get their info.

    Parameters:
        - logical_device / VkDevice / Logical device of the Vulkan instance.

    Returns:
        A vector list containing all created shader modules.
*/
std::vector<ShaderInfo> Vulkan::Shaders::create_all_shader_modules
(
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Creating the shader modules..");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Shader modules creation failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    int total = 0;
    int failed = 0;

    std::vector<ShaderInfo> shader_modules;

    for (const auto &file : std::filesystem::directory_iterator("./shaders"))
    {
        total++;

        const std::string file_name = file.path().filename().string();
        const std::string file_extension = file.path().extension().string();

        if (!std::filesystem::is_regular_file(file.status()))
        {
            Utils::Logs::error_log("- The creation of the shader module \"" + file_name + "\" failed! It's not a valid file!");
            continue;
        }

        if (file_extension != ".frag" && file_extension != ".vert")
        {
            Utils::Logs::error_log("- The creation of the shader module \"" + file_name + "\" failed! It's neither a vertex nor a fragment shader!");
            continue;
        }

        const std::string type = file_extension == ".vert" ? "vert" : "frag";
        const std::vector<char> shader_binaries = Utils::Files::read_binary_file("./shaders/" + file_name);

        const VkShaderModule shader_module = Vulkan::Shaders::create_shader_module(file_name, logical_device, shader_binaries);
        const ShaderInfo shader_info = { type, shader_module };

        shader_modules.emplace_back(shader_info);
        Utils::Logs::log("- Shader module \"" + file_name + "\" (" + Utils::Text::get_memory_address(shader_module) + ") created successfully!");
    }

    if (failed != 0)
        Utils::Logs::error_log("Warning: " + std::to_string(failed) + " shader modules creation failed!");

    Utils::Logs::log(std::to_string(shader_modules.size()) + "/" + std::to_string(total) + " shader modules created successfully!");
    return shader_modules;
}



/*
    Cleanly destroy all shader modules.

    Tasks:
        1) Verify the parameters.
        2) Destroy valid shader modules.

    Parameters:
        - logical_device / VkDevice           / Logical device of the Vulkan instance.
        - shader_modules / vector<ShaderInfo> / Shader modules to destroy.

    Returns:
        No object returned.
*/
void Vulkan::Shaders::destroy_shader_modules
(
    const VkDevice &logical_device,
    std::vector<ShaderInfo> &shader_modules
)
{
    Utils::Logs::log("Destroying " + std::to_string(shader_modules.size()) + " shader modules..");

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Shader modules destruction failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");
        return;
    }

    if (shader_modules.size() < 1)
    {
        Utils::Logs::error_log("Shader modules destruction failed! No shader modules provided!");
        return;
    }

    int failed = 0;
    int i = 0;

    for (ShaderInfo &data : shader_modules)
    {
        i++;
        VkShaderModule shader_module = data.shader_module;

        if (shader_module == VK_NULL_HANDLE)
        {
            Utils::Logs::error_log("- Failed to destroy the shader module #" + std::to_string(i) + "/" + std::to_string(shader_modules.size()) + "! The shader module provided (" + Utils::Text::get_memory_address(shader_module) + ") is not valid!");
            failed++;
            continue;
        }

        vkDestroyShaderModule(logical_device, shader_module, nullptr);
        shader_module = VK_NULL_HANDLE;

        Utils::Logs::log("- Shader module #" + std::to_string(i) + "/" + std::to_string(shader_modules.size()) + " destroyed successfully!");
    }

    if (failed != 0)
        Utils::Logs::error_log("Warning: " + std::to_string(failed) + " shader modules failed to destroy! This might lead to some memory leaks!");

    Utils::Logs::log(std::to_string(shader_modules.size() - failed) + "/" + std::to_string(shader_modules.size()) + " shader modules destroyed successfully!");
    shader_modules.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Vulkan::Shaders::shader_modules_handler::shader_modules_handler
(
    const VkDevice &logical_device
)
    : logical_device(logical_device)
{
    shader_modules = Vulkan::Shaders::create_all_shader_modules(logical_device);
}

Vulkan::Shaders::shader_modules_handler::~shader_modules_handler()
{
    Vulkan::Shaders::destroy_shader_modules(logical_device, shader_modules);
}

std::vector<ShaderInfo> Vulkan::Shaders::shader_modules_handler::get() const
{
    return shader_modules;
}
