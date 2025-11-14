#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <array>

#ifndef VULKAN_VERTEX_HANDLER_HPP
#define VULKAN_VERTEX_HANDLER_HPP

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texture_coordinates;

    static VkVertexInputBindingDescription get_binding_description();
    static std::array<VkVertexInputAttributeDescription, 3> get_attribute_descriptions();
};

#endif
