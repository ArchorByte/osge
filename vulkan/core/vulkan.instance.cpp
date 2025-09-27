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

    // Making the app info.
    // Note: To change most of the info about your game, you can change the config.hpp file in the main folder.
    VkApplicationInfo app_info {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = game_name.c_str();                                                                                          // Game name/title.
    app_info.applicationVersion = VK_MAKE_API_VERSION(game_version_variant, game_version_major, game_version_minor, game_version_patch);    // Game version.
    app_info.pEngineName = "OSGE";                                                                                                          // Game engine name.
    app_info.engineVersion = VK_MAKE_API_VERSION(engine_version_variant, engine_version_major, engine_version_minor, engine_version_patch); // Game engine version.
    app_info.apiVersion = VK_API_VERSION_1_4;                                                                                               // Vulkan API version.

    Uint32 extensions_count = 0;
    const char* const* extensions_list = SDL_Vulkan_GetInstanceExtensions(&extensions_count);

    if (!extensions_list || !extensions_count)
    {
        fatal_error_log("Vulkan instance creation failed! Failed to retrieve the required SDL3 extensions!");
    }

    // Copy them into a vector for Vulkan instance creation
    std::vector<const char*> extensions_vector(extensions_list, extensions_list + extensions_count);

    // Making the app create info.
    VkInstanceCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;                             // Info about the app (defined previously).
    create_info.enabledExtensionCount = extensions_count;                 // Amount of extensions to enable.
    create_info.ppEnabledExtensionNames = extensions_list;                // Enable required extensions.
    create_info.enabledLayerCount = static_cast<uint32_t>(layers.size()); // Amount of layers to enable.
    create_info.ppEnabledLayerNames = layers.data();                      // Enable required layers.

    VkInstance vulkan_instance = VK_NULL_HANDLE;
    VkResult instance_creation = vkCreateInstance(&create_info, nullptr, &vulkan_instance); // Try to create the instance.

    if (instance_creation != VK_SUCCESS)
    {
        fatal_error_log("Vulkan instance creation returned error code " + std::to_string(instance_creation) + ".");
    }

    if (!vulkan_instance || vulkan_instance == VK_NULL_HANDLE)
    {
        fatal_error_log("Vulkan instance creation output \"" + force_string(vulkan_instance) + "\" is not valid!");
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

    if (!vulkan_instance || vulkan_instance == VK_NULL_HANDLE)
    {
        error_log("Vulkan instance destruction failed! The Vulkan instance provided (" + force_string(vulkan_instance) + ") is not valid!");
        return;
    }

    // Destroy the instance and clear the address.
    vkDestroyInstance(vulkan_instance, nullptr);
    vulkan_instance = VK_NULL_HANDLE;

    log("Vulkan instance successfully destroyed!");
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
