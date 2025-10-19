#include "vertex.handler.hpp"

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <array>

// Return the binding description of the Vertex input.
VkVertexInputBindingDescription Vertex::get_binding_description()
{
    const VkVertexInputBindingDescription description
    {
        .binding = 0,             // Set the binding index.
        .stride = sizeof(Vertex), // Size of each vertex.
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    return description;
}

// Return the attribute description of the vertex input.
std::array<VkVertexInputAttributeDescription, 3> Vertex::get_attribute_descriptions()
{
    std::array<VkVertexInputAttributeDescription, 3> descriptions {};

    // Vertex position.
    descriptions[0].binding = 0;                         // Set the binding index.
    descriptions[0].location = 0;                        // Set the position.
    descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;    // Set the color format.
    descriptions[0].offset = offsetof(Vertex, position); // Set the color offset.

    // Vertex color.
    descriptions[1].binding = 0;
    descriptions[1].location = 1;
    descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    descriptions[1].offset = offsetof(Vertex, color);

    // Vertex texture coordinates.
    descriptions[2].binding = 0;
    descriptions[2].location = 2;
    descriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    descriptions[2].offset = offsetof(Vertex, texture_coordinates);

    return descriptions;
}
