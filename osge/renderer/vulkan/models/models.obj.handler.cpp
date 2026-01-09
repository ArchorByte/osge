#include "models.obj.handler.hpp"

#include "../vertex.handler.hpp"
#include "../../logs/logs.handler.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>
#include <vector>
#include <filesystem>
#include <unordered_map>

// Return vertices and indices from an OBJ model.
std::pair<std::vector<Vertex>, std::vector<uint32_t>> load_obj_model
(
    const std::filesystem::path &file_path,
    std::vector<Vertex> &vertices,
    std::vector<uint32_t> &indices
)
{
    const bool file_exists = std::filesystem::exists(file_path);

    if (!file_exists)
    {
        error_log("- The loading of the OBJ model \"" + file_path.string() + "\" failed! No such file or directory!");
        return { vertices, indices };
    }

    const std::string file_name = file_path.filename().string();
    const std::string file_extension = file_path.extension().string();

    if (file_extension != ".obj")
    {
        error_log("- The loading of the OBJ model \"" + file_name + "\" failed ! The file extension is not valid!");
        return { vertices, indices };
    }

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;

    const bool loaded = tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, file_path.string().c_str());

    if (!loaded)
    {
        error_log("- The loading of the OBJ model \"" + file_name + "\" failed with error: " + error + "!");
        return { vertices, indices };
    }

    if (warning.size() > 0)
    {
        error_log("Warning while loading the OBJ model \"" + file_name + "\": " + warning + ".");
    }

    std::unordered_map<Vertex, uint32_t> unique_vertices {};

    for (const auto &shape : shapes)
    {
        for (const auto &index : shape.mesh.indices)
        {
            Vertex vertex {};

            vertex.position =
            {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texture_coordinates =
            {
                attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = { 1.0f, 1.0f, 1.0f };

            if (unique_vertices.count(vertex) == 0)
            {
                unique_vertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(unique_vertices[vertex]);
        }
    }

    return { vertices, indices };
}
