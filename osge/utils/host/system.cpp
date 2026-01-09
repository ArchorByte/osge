#include "utils.host.hpp"

#include "../utils.hpp"
#include "../../config/engine.config.hpp"
#include "../../config/engine.version.hpp"
#include "../../config/game.config.hpp"

#include <string>

/*
    Verify if we are able and allowed to run on this operating system.
    Note: We will automatically trigger a crash if the system is not supported or has been disallowed in the config files.

    Tasks:
        1) Verify that we are not running a 32-bit system. Support dropped on August 24, 2025.
        2) Check if we are running on a supported operating system or not.
        3) Check if we are allowed to run on the system according to the config files.

    Parameters:
        No parameters.

    Returns:
        No object returned.
*/
void Host::check_operating_system_support()
{
    #if !defined(__x86_64__) && !defined(__aarch64__) && !defined(_WIN64)
        Utils::Logs::crash_error_log("The 32-bit architecture is not supported by the game engine! Please upgrade to a 64-bit system to run this program.");
    #endif

    #if defined(__linux__)
        if constexpr (EngineConfig::HANDLE_LINUX_SYSTEMS)
            Utils::Logs::log("Linux system detected!");
        else
            Utils::Logs::crash_error_log("Linux systems have been disallowed by the game developers to run this program!");
    #elif defined(_WIN32)
        if constexpr (EngineConfig::HANDLE_WINDOWS_SYSTEMS)
            Utils::Logs::log("Windows 64-bit system detected!");
        else
            Utils::Logs::crash_error_log("Windows systems have been disallowed by the game developers to run this program!");
    #else
        const int engine_version_variant = EngineVersion::ENGINE_VERSION_VARIANT;
        const int engine_version_major = EngineVersion::ENGINE_VERSION_MAJOR;
        const int engine_version_minor = EngineVersion::ENGINE_VERSION_MINOR;
        const int engine_version_patch = EngineVersion::ENGINE_VERSION_PATCH;

        const std::string engine_version = create_version(engine_version_variant, engine_version_major, engine_version_minor, engine_version_patch);
        Utils::Logs::crash_error_log("Your operating system is not supported by the game engine! Visit https://github.com/ArchorByte/osge to check if a support is planned, or request one.\nEngine version: " + engine_version + ".");
    #endif
}
