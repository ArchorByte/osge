#include "vulkan.vertex.hpp"

#include "libraries/glm/glm.hpp"
#include "libraries/vulkan/vulkan.h"

#include <array>

/*
    Get the vertex input binding description of .

    Tasks:
        1) Make the description.
        2) Return it.

    Parameters:
        No parameters.

    Returns:
        The created binding description.
*/
VkVertexInputBindingDescription Vertex::VertexObj::get_binding_description()
{
    /*
        - binding   / Defines the binding index.
        - stride    / Defines the byte stride between elements within the vertex buffer.
        - inputRate / Defines the rate at which vertex attributes are pulled from buffers.
    */
    const VkVertexInputBindingDescription description
    {
        .binding = 0,
        .stride = sizeof(Vertex::VertexObj),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    return description;
}



/*
    Get the vertex input attribute descriptions.

    Tasks:
        1) Make an array with a description for each attribute (position, color, texture).
        2) Return the array.

    Parameters:
        No parameters.

    Returns:
        The array containing the attribute descriptions.
*/
std::array<VkVertexInputAttributeDescription, 3> Vertex::VertexObj::get_attribute_descriptions()
{
    std::array<VkVertexInputAttributeDescription, 3> descriptions {};

    /*
        Description of the position.
        - binding  / Defines the binding index we are giving the description to.
        - location / Defines the shader input location.
        - format   / Defines the size and type of the attribute.
        - offset   / Defines the starting offset.
    */
    descriptions[0].binding = 0;
    descriptions[0].location = 0;
    descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    descriptions[0].offset = offsetof(Vertex::VertexObj, position);

    /*
        Description of the color.
        - binding  / Defines the binding index we are giving the description to.
        - location / Defines the shader input location.
        - format   / Defines the size and type of the attribute.
        - offset   / Defines the starting offset.
    */
    descriptions[1].binding = 0;
    descriptions[1].location = 1;
    descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    descriptions[1].offset = offsetof(Vertex::VertexObj, color);

    /*
        Description of the texture coordinates.
        - binding  / Defines the binding index we are giving the description to.
        - location / Defines the shader input location.
        - format   / Defines the size and type of the attribute.
        - offset   / Defines the starting offset.
    */
    descriptions[2].binding = 0;
    descriptions[2].location = 2;
    descriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    descriptions[2].offset = offsetof(Vertex::VertexObj, texture_coordinates);

    return descriptions;
}
