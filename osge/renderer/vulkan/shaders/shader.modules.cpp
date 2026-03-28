#include "vulkan.shaders.hpp"

#include "../../../osge/utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a shader module.
    Note: This function was meant to be used by the create_all_shader_modules function only.

    Tasks:
        1) Verify function parameters.
        2) Create shader module.

    Parameters:
        - file_name       / string          / Name of the shader file.
        - logical_device  / VkDevice        / Logical device of the Vulkan instance.
        - shader_binaries / shader_binaries / Binaries of the shader.

    Returns:
        The created shader module.
*/
VkShaderModule Shaders::create_shader_module
(
    const std::string       &file_name,
    const VkDevice          &logical_device,
    const std::vector<char> &shader_binaries
)
{
    if (shader_binaries.size() < 1)
        Utils::Logs::crash_log("Shader module \"" + file_name + "\" creation failed! No binaries provided!");

    if (Utils::Text::trim(file_name).size() < 1)
        Utils::Logs::crash_log("Shader module \"" + file_name + "\" creation failed! The file name provided is not valid!");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Shader module \"" + file_name + "\" creation failed! The logical device provided (" + Utils::Text::get_memory_address(logical_device) + ") is not valid!");

    /*
        - sType    / Defines the type of the structure.
        - codeSize / Defines the size of the shader binaries.
        - pCode    / Passes the shader binaries.
    */
    const VkShaderModuleCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = shader_binaries.size(),
        .pCode = reinterpret_cast<const uint32_t*>(shader_binaries.data())
    };

    VkShaderModule shader_module = VK_NULL_HANDLE;
    const VkResult module_creation = vkCreateShaderModule(logical_device, &create_info, nullptr, &shader_module);

    if (module_creation != VK_SUCCESS)
        Utils::Logs::crash_log("- Shader module \"" + file_name + "\" creation returned error code " + std::to_string(module_creation) + ".");

    return shader_module;
}



/*
    Create a shader module for each valid shader found in the shaders folder.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Verify each file validity.
        3) Load shaders data, create module and get their info.

    Parameters:
        - logical_device / VkDevice / Logical device of the Vulkan instance.

    Returns:
        A vector list containing all created shader modules.
*/
std::vector<ShaderInfo> Shaders::create_all_shader_modules
(
    const VkDevice &logical_device
)
{
    Utils::Logs::log("Creating the shader modules.. ", false);

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    std::vector<ShaderInfo> shader_modules;

    for (const auto &file : std::filesystem::directory_iterator("./shaders"))
    {
        const std::string file_name = file.path().filename().string();
        const std::string file_extension = file.path().extension().string();

        if (!std::filesystem::is_regular_file(file.status()))
            Utils::Logs::crash_log("Failed! Shader \"" + file_name + "\" is not a valid file.");

        if (file_extension != ".frag" && file_extension != ".vert")
            Utils::Logs::crash_log("Failed ! Shader \"" + file_name + "\" is not a vertex or a fragment shader.");

        const std::string type = file_extension == ".vert" ? "vert" : "frag";
        const std::vector<char> shader_binaries = Utils::Files::read_binary_file("./shaders/" + file_name);

        const VkShaderModule shader_module = Shaders::create_shader_module(file_name, logical_device, shader_binaries);
        const ShaderInfo shader_info = { type, shader_module };

        shader_modules.emplace_back(shader_info);
    }

    Utils::Logs::log("Done! " + std::to_string(shader_modules.size()) + " shader modules loaded.", true);
    return shader_modules;
}



/*
    Destroy some shader modules.

    Tasks:
        1) Verify function parameters.
        2) Destroy all shader modules.
        3) Set all objects to null.
        4) Empty vector list.

    Parameters:
        - logical_device / VkDevice           / Logical device of the Vulkan instance.
        - shader_modules / vector<ShaderInfo> / Shader modules to destroy.

    Returns:
        No object returned.
*/
void Shaders::destroy_shader_modules
(
    const VkDevice          &logical_device,
    std::vector<ShaderInfo> &shader_modules
)
{
    Utils::Logs::log("Destroying " + std::to_string(shader_modules.size()) + " shader modules.. ", false);

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    if (shader_modules.size() < 1)
    {
        Utils::Logs::log("Failed! No shader modules provided.", true);
        return;
    }

    for (ShaderInfo &data : shader_modules)
    {
        VkShaderModule shader_module = data.shader_module;

        if (shader_module == VK_NULL_HANDLE)
            continue;

        vkDestroyShaderModule(logical_device, shader_module, nullptr);
        shader_module = VK_NULL_HANDLE;
    }

    Utils::Logs::log("Done!", true);
    shader_modules.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Shaders::shader_modules_handler::shader_modules_handler
(
    const VkDevice &logical_device
)
    : logical_device(logical_device)
{
    shader_modules = Shaders::create_all_shader_modules(logical_device);
}

Shaders::shader_modules_handler::~shader_modules_handler()
{
    Shaders::destroy_shader_modules(logical_device, shader_modules);
}

std::vector<ShaderInfo> Shaders::shader_modules_handler::get() const
{
    return shader_modules;
}
