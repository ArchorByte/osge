#include "shader.modules.hpp"

#include <vector>
#include <vulkan/vulkan.h>

#ifndef VULKAN_SHADERS_STAGES_HPP
#define VULKAN_SHADERS_STAGES_HPP

std::vector<VkPipelineShaderStageCreateInfo> create_vulkan_shader_stages
(
    const std::vector<ShaderInfo> &shaders_modules
);

#endif
