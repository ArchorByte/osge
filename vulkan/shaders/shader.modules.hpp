#include <vulkan/vulkan.h>
#include <vector>
#include <string>

#ifndef VULKAN_SHADERS_MODULES_HPP
#define VULKAN_SHADERS_MODULES_HPP

///////////////////////////////////////////////////
//////////////////// Structure ////////////////////
///////////////////////////////////////////////////

struct ShaderInfo
{
    std::string shader_type;
    VkShaderModule shader_module;
};

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

VkShaderModule create_vulkan_shader_module
(
    const VkDevice &logical_device,
    const std::vector<char> &binaries,
    std::string &file_name
);

std::vector<ShaderInfo> create_all_vulkan_shader_modules
(
    const VkDevice &logical_device
);

void destroy_vulkan_shader_modules
(
    const VkDevice &logical_device,
    std::vector<ShaderInfo> &shaders_modules
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_ShadersModules
{

public:
    // Constructor.
    Vulkan_ShadersModules
    (
        const VkDevice &logical_device
    );

    // Destructor.
    ~Vulkan_ShadersModules();

    std::vector<ShaderInfo> get() const;

    // Prevent data duplication.
    Vulkan_ShadersModules (const Vulkan_ShadersModules&) = delete;
    Vulkan_ShadersModules &operator = (const Vulkan_ShadersModules&) = delete;

private:
    // We declare the members of the class to store.
    VkDevice logical_device = VK_NULL_HANDLE;
    std::vector<ShaderInfo> shaders_modules;

};

#endif
