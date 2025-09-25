#include "env.system.hpp"

#include "../config/engine.config.hpp"
#include "../config/engine.version.hpp"
#include "../config/game.config.hpp"
#include "../logs/logs.handler.hpp"
#include "../helpers/help.versioning.hpp"

#include <string>

// Verify we are running on a supported and allowed operating system.
// We will automatically exit if we are running on a not supported or unauthorized one.
void check_operating_system_support()
{
    #if !defined(__x86_64__) && !defined(__aarch64__) && !defined(_WIN64)
        fatal_error_log("The 32-bit architecture is not supported by this program! Please upgrade to a 64-bit system to run it.");
    #endif

    #if defined(__linux__)
        if constexpr (EngineConfig::HANDLE_LINUX_SYSTEMS)
            log("Linux system detected!");
        else
            fatal_error_log("Linux systems are not allowed to run this program!");
    #elif defined(_WIN32)
        if constexpr (EngineConfig::HANDLE_WINDOWS_SYSTEMS)
            log("Windows 64-bit system detected!");
        else
            fatal_error_log("Windows systems are not allowed to run this program!");
    #else
        // Engine version.
        const int engine_version_variant = EngineVersion::ENGINE_VERSION_VARIANT;
        const int engine_version_major = EngineVersion::ENGINE_VERSION_MAJOR;
        const int engine_version_minor = EngineVersion::ENGINE_VERSION_MINOR;
        const int engine_version_patch = EngineVersion::ENGINE_VERSION_PATCH;

        // Game version.
        const int game_version_variant = GameConfig::GAME_VERSION_VARIANT;
        const int game_version_major = GameConfig::GAME_VERSION_MAJOR;
        const int game_version_minor = GameConfig::GAME_VERSION_MINOR;
        const int game_version_patch = GameConfig::GAME_VERSION_PATCH;

        const std::string engine_version = create_version(engine_version_variant, engine_version_major, engine_version_minor, engine_version_patch);
        const std::string game_version = create_version(game_version_variant, game_version_major, game_version_minor, game_version_patch);

        fatal_error_log("Your operating system is not supported by this program! Visit https://github.com/ArchorByte/osge to check if a support is planned, or ask for one.\nEngine version: " + engine_version + ".\nGame version: " + game_version + ".");
    #endif
}
