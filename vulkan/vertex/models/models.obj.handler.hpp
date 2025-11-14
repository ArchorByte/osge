#include "../vertex/vertex.handler.hpp"

#include <vector>
#include <filesystem>

#ifndef VULKAN_MODELS_OBJ_HANDLER_HPP
#define VULKAN_MODELS_OBJ_HANDLER_HPP

std::pair<std::vector<Vertex>, std::vector<uint32_t>> load_obj_model
(
    const std::filesystem::path &file_path,
    std::vector<Vertex> &vertices,
    std::vector<uint32_t> &indices
);

#endif
