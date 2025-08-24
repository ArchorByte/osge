#include "env.system.hpp"

#include "../config/engine.config.hpp"
#include "../config/engine.version.hpp"
#include "../config/game.config.hpp"
#include "../logs/logs.handler.hpp"

#include <string>

// Verify we are running on a supported/allowed operating system.
// We will automatically exit the program if we are running on a not support/unauthorized OS.
void check_operating_system_support()
{
    #if defined(__linux__)
        #ifdef HANDLE_LINUX
            #if defined(__x86_64__) || defined(__aarch64__)
                log("Linux 64-bit system detected!");
            #else
                fatal_error_log("The 32-bit architecture is not supported by this program! Please upgrade to a 64-bit system to run it.");
            #endif
        #else
            fatal_error_log("Linux systems are not allowed to run this program!");
        #endif
    #elif defined(_WIN32)
        #ifdef HANDLE_WINDOWS
            #if defined(_WIN64)
                log("Windows 64-bit system detected!");
            #else
                fatal_error_log("The 32-bit architecture is not supported by this program! Please upgrade to a 64-bit system to run it.");
            #endif
        #else
            fatal_error_log("Windows systems are not allowed to run this program!");
        #endif
    #else
        std::string engine_version = std::to_string(ENGINE_VERSION_VARIANT) + "." + std::to_string(ENGINE_VERSION_MAJOR) + "." + std::to_string(ENGINE_VERSION_MINOR) + "." + std::to_string(ENGINE_VERSION_PATCH);
        std::string game_version = std::to_string(GAME_VERSION_VARIANT) + "." + std::to_string(GAME_VERSION_MAJOR) + "." + std::to_string(GAME_VERSION_MINOR) + "." + std::to_string(GAME_VERSION_PATCH);

        fatal_error_log("Your operating system is not supported by this program! Visit https://github.com/ArchorByte/osge to check if a support is planned, or ask for one.\nEngine version: " + engine_version + ".\nGame version: " + game_version + ".");
    #endif
}
