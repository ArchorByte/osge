#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <array>
#include <cstdint>

#ifndef VULKAN_VERTEX_HPP
#define VULKAN_VERTEX_HPP

struct Vertex
{
    glm::vec2 position;
    glm::vec3 color;

    static VkVertexInputBindingDescription get_binding_description();
    static std::array<VkVertexInputAttributeDescription, 2> get_attribute_descriptions();
};

const std::vector<Vertex> vertices =
{
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices =
{
    0, 1, 2, 2, 3, 0
};

#endif
