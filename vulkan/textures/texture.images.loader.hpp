#include <vulkan/vulkan.h>
#include <string>
#include <stb/stb_image.h>
#include <vector>

#ifndef VULKAN_TEXTURE_IMAGES_LOADER_HPP
#define VULKAN_TEXTURE_IMAGES_LOADER_HPP

///////////////////////////////////////////////////
//////////////////// Structure ////////////////////
///////////////////////////////////////////////////

struct TextureImageInfo
{
    std::string name;
    int width;
    int height;
    int channels;
    uint32_t mip_levels;
    stbi_uc* pixels;
    VkDeviceSize size;
};

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

std::vector<TextureImageInfo> load_texture_images();

void free_texture_images
(
    std::vector<TextureImageInfo> &images
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_LoadedTextureImages
{

public:
    // Constructor.
    Vulkan_LoadedTextureImages();

    // Destructor.
    ~Vulkan_LoadedTextureImages();

    std::vector<TextureImageInfo> get() const;

    // Prevent data duplication.
    Vulkan_LoadedTextureImages (const Vulkan_LoadedTextureImages&) = delete;
    Vulkan_LoadedTextureImages &operator = (const Vulkan_LoadedTextureImages&) = delete;

private:
    // We declare the members of the class to store.
    std::vector<TextureImageInfo> texture_image_info;

};

#endif
