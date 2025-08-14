#include <vector>

#ifndef VULKAN_CORE_LAYERS_HPP
#define VULKAN_CORE_LAYERS_HPP

void check_vulkan_layers_compatibility
(
    const std::vector<const char*> &layers
);

#endif
