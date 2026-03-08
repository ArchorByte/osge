#include "vulkan.models.hpp"

#include "../vertex/vulkan.vertex.hpp"
#include "../../../utils/utils.hpp"

#include <filesystem>
#include <string>
#include <vector>

/*
    Load all 3D models' vertices and indices into the engine.

    Tasks:
        1) Get all files in the local models folder.
        2) For each file:
            - Get basic information about it (path, name and extension).
            - Verify it's a valid file.
            - Try to load it using the proper loading method depending on its extension.

    Parameters:
        - indices  / uint32_t  / Separates all vertices by shapes.
        - vertices / VertexObj / Engines' 3D shape definitions.

    Returns:
        No object returned.
*/
void Models::load_3d_models
(
    std::vector<uint32_t>          &indices,
    std::vector<Vertex::VertexObj> &vertices
)
{
    Utils::Logs::log("Loading 3D models..", true);

    int total = 0;
    int failed = 0;

    for (const auto &file : std::filesystem::directory_iterator("./models"))
    {
        total++;

        const std::filesystem::path file_path = file.path();
        const std::string file_name = file_path.filename().string();
        const std::string file_extension = file_path.extension().string();

        if (!std::filesystem::is_regular_file(file.status()))
        {
            Utils::Logs::log("- Loading of model \"" + file_name + "\" failed! Not a valid file.", true);
            failed++;
            continue;
        }

        if (file_extension == ".obj")
        {
            Models::load_obj_model(file_path, indices, vertices);
            continue;
        }

        Utils::Logs::log("- Loading of model \"" + file_name + "\" failed! File extension (\"" + file_extension + "\") not supported by the engine. Supported extensions: .obj.", true);
        failed++;
    }

    if (failed > 0)
        Utils::Logs::log("Models loading completed! Warning: " + std::to_string(failed) + "/" + std::to_string(total) + " models failed to load.", true);
    else
        Utils::Logs::log("Models loading completed! " + std::to_string(total) + " models loaded.", true);
}
