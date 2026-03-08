#include "vulkan.models.hpp"

#define TINYOBJLOADER_IMPLEMENTATION

#include "../vertex/vulkan.vertex.hpp"
#include "../../../utils/utils.hpp"
#include "../../../../libraries/tinyobjloader/tiny_obj_loader.h"

#include <filesystem>
#include <unordered_map>
#include <vector>

/*
    Load .obj models' vertices and indices into the engine.

    Tasks:
        1) Verify function parameters.
        2) Try to load the model using tinyobjloader.
        3) Retrieve output data, sort the information, and add it to the engine definitions.

    Parameters:
        - file_path / path              / Path to the .obj file to load.
        - indices   / vector<uint32_t>  / Separates all vertices by shapes.
        - vertices  / vector<VertexObj> / Engines' 3D shape definitions.

    Returns:
        No object returned.
*/
void Models::load_obj_model
(
    const std::filesystem::path    &file_path,
    std::vector<uint32_t>          &indices,
    std::vector<Vertex::VertexObj> &vertices
)
{
    const bool file_exists = std::filesystem::exists(file_path);
    const std::string file_name = file_path.filename().string();
    const std::string file_extension = file_path.extension().string();

    if (!file_exists)
    {
        Utils::Logs::log("- Loading of .obj model \"" + file_path.string() + "\" failed! No such file or directory.", true);
        return;
    }

    if (file_extension != ".obj")
    {
        Utils::Logs::log("- Loading of .obj model \"" + file_name + "\" failed! Invalid file extension.", true);
        return;
    }

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;

    const bool loaded = tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, file_path.string().c_str());

    if (!loaded)
    {
        Utils::Logs::log("- Loading of .obj model \"" + file_name + "\" failed with error: \"" + error + "\"!", true);
        return;
    }

    if (warning.size() > 0)
        Utils::Logs::log("Warning while loading .obj model \"" + file_name + "\": " + warning + ".", true);

    std::unordered_map<Vertex::VertexObj, uint32_t> unique_vertices {};

    for (const tinyobj::shape_t &shape : shapes)
    {
        for (const tinyobj::index_t &index : shape.mesh.indices)
        {
            Vertex::VertexObj vertex {};

            /*
                - position       / Position of the object.
            */
            vertex.position =
            {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            /*
                - texture_coordinates / Position of the texture to apply.
            */
            vertex.texture_coordinates =
            {
                attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 1]
            };

            /*
                - Set the color of the object by default.
            */
            vertex.color = { 1.0f, 1.0f, 1.0f };

            if (unique_vertices.count(vertex) == 0)
            {
                unique_vertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(unique_vertices[vertex]);
        }
    }
}
