#include "vulkan.instance.hpp"

#include "../../config/game.config.hpp"
#include "../../config/engine.config.hpp"
#include "../../config/engine.version.hpp"
#include "../../logs/logs.handler.hpp"
#include "../../helpers/help.text.format.hpp"

#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <cstdint>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a Vulkan instance.
VkInstance create_vulkan_instance
(
    const std::vector<const char*> &layers
)
{
    log("Creating a Vulkan instance..");
    std::string game_name = std::string(GameConfig::GAME_TITLE);

    if constexpr (EngineConfig::DEBUG_MODE)
        game_name += " - Debug Mode";

    // Game version.
    const int game_version_variant = GameConfig::GAME_VERSION_VARIANT;
    const int game_version_major = GameConfig::GAME_VERSION_MAJOR;
    const int game_version_minor = GameConfig::GAME_VERSION_MINOR;
    const int game_version_patch = GameConfig::GAME_VERSION_PATCH;

    // Engine version.
    const int engine_version_variant = EngineVersion::ENGINE_VERSION_VARIANT;
    const int engine_version_major = EngineVersion::ENGINE_VERSION_MAJOR;
    const int engine_version_minor = EngineVersion::ENGINE_VERSION_MINOR;
    const int engine_version_patch = EngineVersion::ENGINE_VERSION_PATCH;

    // Note: To change most of your game's info, you can change the config.hpp file in the main folder.
    VkApplicationInfo app_info
    {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = game_name.c_str(),
        .applicationVersion = VK_MAKE_API_VERSION(game_version_variant, game_version_major, game_version_minor, game_version_patch),
        .pEngineName = "OSGE - Open Source Game Engine",
        .engineVersion = VK_MAKE_API_VERSION(engine_version_variant, engine_version_major, engine_version_minor, engine_version_patch),
        .apiVersion = VK_API_VERSION_1_4 // Version of the API that we are using.
    };

    Uint32 extensions_count = 0;
    const char* const* extensions_list = SDL_Vulkan_GetInstanceExtensions(&extensions_count);

    if (!extensions_list || !extensions_count)
    {
        fatal_error_log("Vulkan instance creation failed! Failed to retrieve the required SDL3 extensions!");
    }

    VkInstanceCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = static_cast<uint32_t>(layers.size()), // Amount of layers to enable.
        .ppEnabledLayerNames = layers.data(),                      // Pass the layers list.
        .enabledExtensionCount = extensions_count,                 // Amount of extensions to enable.
        .ppEnabledExtensionNames = extensions_list                 // List of the required extensions.
    };

    VkInstance vulkan_instance = VK_NULL_HANDLE;
    const VkResult instance_creation = vkCreateInstance(&create_info, nullptr, &vulkan_instance);

    if (instance_creation != VK_SUCCESS)
    {
        fatal_error_log("Vulkan instance creation returned error code " + std::to_string(instance_creation) + ".");
    }

    if (vulkan_instance == VK_NULL_HANDLE)
    {
        fatal_error_log("Vulkan instance creation output (" + force_string(vulkan_instance) + ") is not valid!");
    }

    log("Vulkan instance " + force_string(vulkan_instance) + " created successfully!");
    return vulkan_instance;
}

// Destroy a Vulkan instance.
void destroy_vulkan_instance
(
    VkInstance &vulkan_instance
)
{
    log("Destroying the " + force_string(vulkan_instance) + " Vulkan instance..");

    if (vulkan_instance == VK_NULL_HANDLE)
    {
        error_log("Vulkan instance destruction failed! The Vulkan instance provided (" + force_string(vulkan_instance) + ") is not valid!");
        return;
    }

    vkDestroyInstance(vulkan_instance, nullptr);
    vulkan_instance = VK_NULL_HANDLE;

    log("Vulkan instance destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_Instance::Vulkan_Instance
(
    const std::vector<const char*> &layers
)
{
    vulkan_instance = create_vulkan_instance(layers);
}

// Destructor.
Vulkan_Instance::~Vulkan_Instance()
{
    destroy_vulkan_instance(vulkan_instance);
}

VkInstance Vulkan_Instance::get() const
{
    return vulkan_instance;
}
