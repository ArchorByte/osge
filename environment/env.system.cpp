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
        #ifdef HANDLE_LINUX
            log("Linux system detected!");
        #else
            fatal_error_log("Linux systems are not allowed to run this program!");
        #endif
    #elif defined(_WIN32)
        #ifdef HANDLE_WINDOWS
            log("Windows 64-bit system detected!");
        #else
            fatal_error_log("Windows systems are not allowed to run this program!");
        #endif
    #else
        const std::string engine_version = create_version(ENGINE_VERSION_VARIANT, ENGINE_VERSION_MAJOR, ENGINE_VERSION_MINOR, ENGINE_VERSION_PATCH);
        const std::string game_version = create_version(GAME_VERSION_VARIANT, GAME_VERSION_MAJOR, GAME_VERSION_MINOR, GAME_VERSION_PATCH);

        fatal_error_log("Your operating system is not supported by this program! Visit https://github.com/ArchorByte/osge to check if a support is planned, or ask for one.\nEngine version: " + engine_version + ".\nGame version: " + game_version + ".");
    #endif
}
