#define GLM_ENABLE_EXPERIMENTAL
#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
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

    bool operator==(const Vertex& other) const
    {
        return position == other.position && color == other.color && texture_coordinates == other.texture_coordinates;
    }
};

namespace std
{
    template<> struct hash<Vertex>
    {
        size_t operator()(Vertex const& vertex) const
        {
            return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texture_coordinates) << 1);
        }
    };
}

#endif
