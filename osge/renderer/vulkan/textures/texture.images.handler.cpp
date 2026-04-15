#include "vulkan.textures.hpp"

#include "libraries/stb/stb_image.h"
#include "libraries/vulkan/vulkan.h"
#include "../buffers/vulkan.buffers.hpp"
#include "../images/vulkan.images.hpp"
#include "../../../utils/utils.hpp"

#include <filesystem>
#include <string>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create images using texture information.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.
    Warning: All textures with invalid info will be ignored.

    Tasks:
        1) Verify function parameters.
        2) Create a texture image using all texture information.

    Parameters:
        - command_pool          / Handles memory allocation of command buffers.
        - graphics_queue        / Handles all graphics commands and calls.
        - logical_device        / Logical device of the Vulkan instance.
        - physical_device       / Physical device used to run this Vulkan instance.
        - texture_image_buffers / Buffers allocated to texture images.
        - texture_image_info    / Information about all texture images.

    Returns:
        A vector list contain all texture images.
*/
std::vector<TextureImageData> Textures::create_texture_images
(
    const VkCommandPool                 &command_pool,
    const VkQueue                       &graphics_queue,
    const VkDevice                      &logical_device,
    const VkPhysicalDevice              &physical_device,
    const std::vector<Buffer>           &texture_image_buffers,
    const std::vector<TextureImageInfo> &texture_image_info
)
{
    Utils::Logs::log("Creating " + std::to_string(texture_image_info.size()) + " texture images.. ", false);

    if (command_pool == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Command pool invalid.");

    if (graphics_queue == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Graphics queue invalid.");

    if (logical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Logical device invalid.");

    if (physical_device == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Physical device invalid.");

    if (texture_image_buffers.size() < 1)
        Utils::Logs::crash_log("Failed! No texture image buffers provided.");

    if (texture_image_info.size() < 1)
        Utils::Logs::crash_log("Failed! No texture image info provided.");

    std::vector<TextureImageData> texture_images;
    texture_images.reserve(texture_image_info.size());

    int i = 0;

    for (const TextureImageInfo &info : texture_image_info)
    {
        i++;

        const std::string texture_name = info.name;
        const uint32_t mip_levels = info.mip_levels;
        const int image_width = info.width;
        const int image_height = info.height;

        if (Utils::Text::trim(texture_name).size() < 1)
            continue;

        if (mip_levels < 1)
            continue;

        if (image_height < 1)
            continue;

        if (image_width < 1)
            continue;

        const std::pair<VkImage, VkDeviceMemory> texture_image = Images::create_image(VK_FORMAT_R8G8B8A8_SRGB, image_height, VK_IMAGE_TILING_OPTIMAL, logical_device, mip_levels, physical_device, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, image_width);
        const TextureImageData image { texture_name, texture_image.first, texture_image.second, mip_levels };

        Images::transition_image_layout(command_pool, VK_FORMAT_R8G8B8A8_SRGB, graphics_queue, texture_image.first, logical_device, mip_levels, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_UNDEFINED);
        Buffers::copy_buffer_to_texture_image(command_pool, graphics_queue, logical_device, texture_image_buffers[i - 1].buffer, texture_image.first, info);
        Textures::generate_mipmaps(command_pool, VK_FORMAT_R8G8B8A8_SRGB, graphics_queue, image_height, texture_image.first, logical_device, mip_levels, physical_device, image_width);

        texture_images.emplace_back(image);
    }

    Utils::Logs::log("Done!", true);
    return texture_images;
}



/*
    Try to load all files from the local textures folder.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Get all files in the directory.
        2) For each file:
            - Verify file validity.
            - Check file extension.
            - Load file.
            - Verify file properties.
            - Register the file information.

    Parameters:
        No parameters.

    Returns:
        A vector list containing all loaded textures.
*/
std::vector<TextureImageInfo> Textures::load_textures()
{
    Utils::Logs::log("Loading textures from local folder.. ", false);

    std::vector<TextureImageInfo> texture_info;
    int total = 0;

    for (const auto &file : std::filesystem::directory_iterator("./textures/"))
    {
        total++;

        const std::string file_extension = file.path().extension().string();
        const std::string file_path = file.path().string();
        const std::string file_name = file.path().filename().string();

        if (!std::filesystem::is_regular_file(file.status()))
            continue;

        if (file_extension != ".png" && file_extension != ".jpg" && file_extension != ".jpeg")
            continue;

        int width;
        int height;
        int channels;
        const int bytes = 4;

        stbi_uc* pixels = stbi_load(file_path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        const VkDeviceSize image_size = width * height * bytes;
        const uint32_t mip_levels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;

        if (!pixels)
            continue;

        if (width < 1)
            continue;

        if (height < 1)
            continue;

        const TextureImageInfo image_info = { file_name, channels, height, mip_levels, pixels, image_size, width };
        texture_info.emplace_back(image_info);
    }

    if (texture_info.size() < total)
        Utils::Logs::log("Done! Warning: " + std::to_string(total - texture_info.size()) + " textures couldn't be loaded.", true);
    else
        Utils::Logs::log("Done!", true);

    return texture_info;
}



/*
    Destroy some texture images.

    Tasks:
        1) Verify function parameters.
        2) Destroy all valid texture images.
        3) Set objects to null.

    Parameters:
        - logical_device / VkDevice                 / Logical device of the Vulkan instance.
        - texture_images / vector<TextureImageData> / Texture images to destroy.

    Returns:
        No object returned.
*/
void Textures::destroy_texture_images
(
    const VkDevice                &logical_device,
    std::vector<TextureImageData> &texture_images
)
{
    Utils::Logs::log("Destroying " + std::to_string(texture_images.size()) + " texture images.. ", false);

    if (logical_device == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Logical device invalid.", true);
        return;
    }

    if (texture_images.size() < 1)
    {
        Utils::Logs::log("Done!", true);
        return;
    }

    int failed = 0;
    int i = 0;

    for (TextureImageData &texture_image : texture_images)
    {
        i++;

        const std::string texture_name = texture_image.name;
        VkImage image = texture_image.texture_image;
        VkDeviceMemory image_memory = texture_image.image_memory;

        if (image == VK_NULL_HANDLE)
        {
            failed++;
            continue;
        }

        if (image_memory == VK_NULL_HANDLE)
        {
            failed++;
            continue;
        }

        vkDestroyImage(logical_device, image, nullptr);
        image = VK_NULL_HANDLE;

        vkFreeMemory(logical_device, image_memory, nullptr);
        image_memory = VK_NULL_HANDLE;
    }

    if (failed > 0)
        Utils::Logs::log("Done! Warning: " + std::to_string(failed) + " destructions failed.", true);
    else
        Utils::Logs::log("Done!", true);

    texture_images.clear();
}



/*
    Free some textures loaded using stb.

    Tasks:
        1) Verify function parameters.
        2) Free all valid textures.
        3) Set objects to null.

    Parameters:
        - all_texture_information / vector<TextureImageInfo> / Information about all textures to free.

    Returns:
        No object returned.
*/
void Textures::free_textures
(
    std::vector<TextureImageInfo> &all_texture_information
)
{
    Utils::Logs::log("Freeing " + std::to_string(all_texture_information.size()) + " textures.. ", false);

    if (all_texture_information.size() < 1)
    {
        Utils::Logs::log("Done!", true);
        return;
    }

    int failed = 0;
    int i = 0;

    for (TextureImageInfo &texture_information : all_texture_information)
    {
        i++;
        stbi_uc* texture_image = texture_information.pixels;

        if (texture_image == nullptr)
        {
            failed++;
            continue;
        }

        stbi_image_free(texture_image);
        texture_information.pixels = nullptr;
    }

    if (failed > 0)
        Utils::Logs::log("Done! Warning: " + std::to_string(failed) + " failed to free.", true);
    else
        Utils::Logs::log("Done!", true);

    all_texture_information.clear();
}



///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Textures::texture_images_handler::texture_images_handler
(
    const VkCommandPool                 &command_pool,
    const VkQueue                       &graphics_queue,
    const VkDevice                      &logical_device,
    const VkPhysicalDevice              &physical_device,
    const std::vector<Buffer>           &texture_image_buffers,
    const std::vector<TextureImageInfo> &texture_image_info
)
    : logical_device(logical_device)
{
    texture_images = create_texture_images(command_pool, graphics_queue, logical_device, physical_device, texture_image_buffers, texture_image_info);
}

Textures::texture_images_handler::~texture_images_handler()
{
    destroy_texture_images(logical_device, texture_images);
}

std::vector<TextureImageData> Textures::texture_images_handler::get() const
{
    return texture_images;
}



Textures::loaded_textures_handler::loaded_textures_handler()
{
    texture_image_information = load_textures();
}

Textures::loaded_textures_handler::~loaded_textures_handler()
{
    free_textures(texture_image_information);
}

std::vector<TextureImageInfo> Textures::loaded_textures_handler::get() const
{
    return texture_image_information;
}
