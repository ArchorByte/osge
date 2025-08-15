#include "vertex.handler.hpp"

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <array>

// Return the binding description of the Vertex input.
VkVertexInputBindingDescription Vertex::get_binding_description()
{
    VkVertexInputBindingDescription description {};
    description.binding = 0;                             // Set the binding index.
    description.stride = sizeof(Vertex);                 // Pass the size of the shader data.
    description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // Set the input rate as "per-vertex" and not "per-instance".

    return description;
}

// Return the attribute description of the Vertex input.
std::array<VkVertexInputAttributeDescription, 2> Vertex::get_attribute_descriptions()
{
    std::array<VkVertexInputAttributeDescription, 2> descriptions {};

    // 2D positions.
    descriptions[0].binding = 0;                         // Set the binding index.
    descriptions[0].location = 0;                        // Set the position.
    descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;    // Set the format as two 32-bits elements.
    descriptions[0].offset = offsetof(Vertex, position); // Set the color offset.

    // 3D positions.
    descriptions[1].binding = 0;                         // Set the binding index.
    descriptions[1].location = 1;                        // Set the position.
    descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT; // Set the format as three 32-bits elements.
    descriptions[1].offset = offsetof(Vertex, color);    // Set the color offset.

    return descriptions;
}
