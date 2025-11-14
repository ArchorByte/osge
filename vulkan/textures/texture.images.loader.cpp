#include "texture.images.loader.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Load all images available in the textures directory.
std::vector<TextureImageInfo> load_texture_images()
{
    log("Loading texture images..");

    std::vector<TextureImageInfo> texture_info;
    int total = 0;

    for (const auto &file : fs::directory_iterator("./textures/"))
    {
        total++;

        const std::string file_extension = file.path().extension().string();
        const std::string file_path = file.path().string();
        const std::string file_name = file.path().filename().string();

        if (!fs::is_regular_file(file.status()))
        {
            error_log("Failed to load texture \"" + file_name + "\"! It's not a valid file!");
            continue;
        }

        if (file_extension != ".png" && file_extension != ".jpg" && file_extension != ".jpeg")
        {
            error_log("Failed to load texture \"" + file_name + "\"! The file extension (\"" + file_extension + "\") is not supported by the engine! Supported extensions: .png, .jpg, .jpeg.");
            continue;
        }

        int width;
        int height;
        int channels;
        const int bytes = 4; // Amount of bytes per each pixel.

        stbi_uc* pixels = stbi_load(file_path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        const VkDeviceSize image_size = width * height * bytes;

        if (!pixels)
        {
            error_log("- Failed to load the texture image \"" + file_name + "\"!");
            continue;
        }

        if (width < 1)
        {
            error_log("- Failed to load the texture image \"" + file_name + "\"! The image width (" + std::to_string(width) + ") is not valid!");
            continue;
        }

        if (height < 1)
        {
            error_log("- Failed to load the texture image \"" + file_name + "\"! The image height (" + std::to_string(height) + ") is not valid!");
            continue;
        }

        const TextureImageInfo image_info =
        {
            file_name,
            width,
            height,
            channels, // Color channels of the image.
            pixels,   // Image itself.
            image_size
        };

        texture_info.emplace_back(image_info);
        log("- Texture image \"" + file_name + "\" (" + force_string(pixels) + ") loaded successfully!");
    }

    if (texture_info.size() < total)
    {
        error_log("- Warning: " + std::to_string(total - texture_info.size()) + " texture images failed to load!");
    }

    log(texture_info.size() + "/" + std::to_string(total) + " texture images loaded successfully!");
    return texture_info;
}

// Free some images loaded from the textures directory.
void free_texture_images
(
    std::vector<TextureImageInfo> &all_texture_info
)
{
    log("Freeing " + std::to_string(all_texture_info.size()) + " texture images..");

    if (all_texture_info.size() < 1)
    {
        error_log("Texture images failed to free! No texture image info were provided!");
        return;
    }

    int failed = 0;
    int i = 0;

    for (TextureImageInfo &texture_info : all_texture_info)
    {
        i++;
        stbi_uc* texture_image = texture_info.pixels;

        if (texture_image == nullptr)
        {
            error_log("- Failed to free the texture image #" + std::to_string(i) + "/" + std::to_string(all_texture_info.size()) + "! The texture image provided (" + force_string(texture_image) + ") is not valid!");
            failed++;
            continue;
        }

        stbi_image_free(texture_image);
        texture_info.pixels = nullptr;

        log("- Texture image #" + std::to_string(i) + "/" + std::to_string(all_texture_info.size()) + " freed successfully!");
    }

    if (failed > 0)
    {
        error_log("Warning: " + std::to_string(failed) + " texture images failed to free! This might lead to some memory leaks!");
    }

    log(std::to_string(all_texture_info.size() - failed) + "/" + std::to_string(all_texture_info.size()) + " texture images freed successfully!");
    all_texture_info.clear();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_LoadedTextureImages::Vulkan_LoadedTextureImages()
{
    texture_image_info = load_texture_images();
}

// Destructor.
Vulkan_LoadedTextureImages::~Vulkan_LoadedTextureImages()
{
    free_texture_images(texture_image_info);
}

std::vector<TextureImageInfo> Vulkan_LoadedTextureImages::get() const
{
    return texture_image_info;
}
