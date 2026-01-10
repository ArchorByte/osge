#ifndef VULKAN_SHADERS_HPP
#define VULKAN_SHADERS_HPP

#include <libraries/vulkan/vulkan.h>
#include <string>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Structure ////////////////////
///////////////////////////////////////////////////

struct ShaderInfo
{
    std::string shader_type;
    VkShaderModule shader_module;
};

///////////////////////////////////////////////////
//////////////////// Namespace ////////////////////
///////////////////////////////////////////////////

namespace Vulkan::Shaders
{
    //////////////////////////////
    ///// shader.modules.cpp /////
    //////////////////////////////

    VkShaderModule create_shader_module
    (
        const std::string &file_name,
        const VkDevice &logical_device,
        const std::vector<char> &shader_binaries
    );

    std::vector<ShaderInfo> create_all_shader_modules
    (
        const VkDevice &logical_device
    );

    void destroy_shader_modules
    (
        const VkDevice &logical_device,
        std::vector<ShaderInfo> &shaders_modules
    );

    /////////////////////////////
    ///// shader.stages.cpp /////
    /////////////////////////////

    std::vector<VkPipelineShaderStageCreateInfo> create_shader_stages
    (
        const std::vector<ShaderInfo> &shader_modules
    );

    ///////////////////////////////////////////////
    //////////////////// Class ////////////////////
    ///////////////////////////////////////////////

    class shader_modules_handler
    {
        public:
            shader_modules_handler
            (
                const VkDevice &logical_device
            );

            ~shader_modules_handler();
            std::vector<ShaderInfo> get() const;

            // Prevent data duplication.
            shader_modules_handler (const shader_modules_handler&) = delete;
            shader_modules_handler &operator = (const shader_modules_handler&) = delete;
        private:
            VkDevice logical_device = VK_NULL_HANDLE;
            std::vector<ShaderInfo> shader_modules;
    };
}

#endif
