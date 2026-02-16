#include "vulkan.core.hpp"

#include "config/game.config.hpp"
#include "config/engine.config.hpp"
#include "config/engine.version.hpp"
#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vector>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a Vulkan instance.
    Note: You should use the pre-made class to handle the Vulkan instance rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Retrieve and enable all required SDL3 extensions.
        3) Create the Vulkan instance.

    Parameters:
        - layers / vector<const char*> / Layers to enable.

    Returns:
        The created Vulkan instance.
*/
VkInstance Core::create_vulkan_instance
(
    const std::vector<const char*> &layers
)
{
    Utils::Logs::log("Creating Vulkan instance.. ", true);
    std::string game_name = std::string(GameConfig::GAME_TITLE);

    if constexpr (EngineConfig::DEBUG_MODE)
        game_name += " - Debug Mode";

    const int game_version_variant = GameConfig::GAME_VERSION_VARIANT;
    const int game_version_major = GameConfig::GAME_VERSION_MAJOR;
    const int game_version_minor = GameConfig::GAME_VERSION_MINOR;
    const int game_version_patch = GameConfig::GAME_VERSION_PATCH;

    const int engine_version_variant = EngineVersion::ENGINE_VERSION_VARIANT;
    const int engine_version_major = EngineVersion::ENGINE_VERSION_MAJOR;
    const int engine_version_minor = EngineVersion::ENGINE_VERSION_MINOR;
    const int engine_version_patch = EngineVersion::ENGINE_VERSION_PATCH;

    /*
        - sType              / Defines the type of the structure.
        - pApplicationName   / Defines the name of the application. We use the game name defined in the game.config.hpp file.
        - applicationVersion / Defines the version of the application. We use the game version numbers defined in the game.config.hpp file.
        - pEngineName        / Defines the name of the game engine.
        - engineVersion      / Defines the version of the game engine.
        - apiVersion         / Defines the version of the Vulkan API we are using.
    */
    const VkApplicationInfo app_info
    {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = game_name.c_str(),
        .applicationVersion = VK_MAKE_API_VERSION(game_version_variant, game_version_major, game_version_minor, game_version_patch),
        .pEngineName = "OSGE - Open Source Game Engine",
        .engineVersion = VK_MAKE_API_VERSION(engine_version_variant, engine_version_major, engine_version_minor, engine_version_patch),
        .apiVersion = VK_API_VERSION_1_4
    };

    Uint32 extensions_count = 0;
    const char* const* extensions_list = SDL_Vulkan_GetInstanceExtensions(&extensions_count);

    if (!extensions_list || !extensions_count)
        Utils::Logs::crash_log("Failed! Required SDL3 extensions listing failed.");

    /*
        - sType                   / Defines the type of the structure.
        - pApplicationInfo        / Provides some information about the application.
        - enabledLayerCount       / Amount of layers to enable.
        - ppEnabledLayerNames     / Passes the name of all layers to enable.
        - enabledExtensionCount   / Amount of extensions to enable.
        - ppEnabledExtensionNames / Passes the name of all extensions to enable.
    */
    const VkInstanceCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = static_cast<uint32_t>(layers.size()),
        .ppEnabledLayerNames = layers.data(),
        .enabledExtensionCount = extensions_count,
        .ppEnabledExtensionNames = extensions_list
    };

    VkInstance vulkan_instance = VK_NULL_HANDLE;
    const VkResult instance_creation = vkCreateInstance(&create_info, nullptr, &vulkan_instance);

    if (instance_creation != VK_SUCCESS)
        Utils::Logs::crash_log("Failed! Creation returned error code -> " + std::to_string(instance_creation) + ".");

    Utils::Logs::log("Done! Memory address -> " + Utils::Text::get_memory_address(vulkan_instance) + ".", true);
    return vulkan_instance;
}



/*
    Destroy a Vulkan instance.

    Tasks:
        1) Verify function parameters.
        2) Destroy the Vulkan instance.
        3) Set object to null.

    Parameters:
        - vulkan_instance / VkInstance / Vulkan instance to destroy.

    Returns:
        No object returned.
*/
void Core::destroy_vulkan_instance
(
    VkInstance &vulkan_instance
)
{
    Utils::Logs::log("Destroying Vulkan instance (" + Utils::Text::get_memory_address(vulkan_instance) + ").. ", false);

    if (vulkan_instance == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Instance invalid.", true);
        return;
    }

    vkDestroyInstance(vulkan_instance, nullptr);
    vulkan_instance = VK_NULL_HANDLE;

    Utils::Logs::log("Done!", true);
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Core::instance_handler::instance_handler
(
    const std::vector<const char*> &layers
)
{
    vulkan_instance = Core::create_vulkan_instance(layers);
}

Core::instance_handler::~instance_handler()
{
    Core::destroy_vulkan_instance(vulkan_instance);
}

VkInstance Core::instance_handler::get() const
{
    return vulkan_instance;
}
