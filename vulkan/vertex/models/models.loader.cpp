#include "models.loader.hpp"

#include "models.obj.handler.hpp"
#include "../vertex.handler.hpp"
#include "../../logs/logs.handler.hpp"

#include <vector>
#include <string>
#include <filesystem>

// Load all 3D models as vertices and indices into the engine.
void load_3d_models
(
    std::vector<Vertex> &vertices,
    std::vector<uint32_t> &indices
)
{
    log("Loading 3D models..");

    int total = 0;
    int succeeded = 0;

    for (const auto &file : std::filesystem::directory_iterator("./models"))
    {
        total++;

        const std::filesystem::path file_path = file.path();
        const std::string file_name = file_path.filename().string();
        const std::string file_extension = file_path.extension();

        if (!std::filesystem::is_regular_file(file.status()))
        {
            error_log("- The loading of the model \"" + file_name + "\" failed! It's not a valid file!");
            continue;
        }

        if (file_extension == ".obj")
        {
            load_obj_model(file_path, vertices, indices);
        }
        else
        {
            error_log("- The loading of the model \"" + file_name + "\" failed! The file extension (\"" + file_extension + "\") is not supported by the engine! Supported extension: .obj.");
            continue;
        }

        succeeded++;
        log("- Model \"" + file_name + "\" loaded successfully!");
    }

    if (succeeded < total)
    {
        error_log("Warning: " + std::to_string(total - succeeded) + " models failed to load!");
    }

    log(std::to_string(succeeded) + "/" + std::to_string(total) + " models loaded successfully!");
}
