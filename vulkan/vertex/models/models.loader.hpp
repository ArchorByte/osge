#include "../vertex/vertex.handler.hpp"

#include <vector>

#ifndef VULKAN_MODELS_LOADER_HPP
#define VULKAN_MODELS_LOADER_HPP

void load_3d_models
(
    std::vector<Vertex> &vertices,
    std::vector<uint32_t> &indices
);

#endif
