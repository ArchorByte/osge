#include "env.system.hpp"

#include "../config/engine.config.hpp"
#include "../logs/logs.handler.hpp"

#include <string>

// Verify we are running on a supported/allowed operating system.
// We will automatically exit the program if we are running on a not support/unauthorized OS.
void check_operating_system_support()
{
    #if defined(__linux__)
        #ifdef HANDLE_LINUX
            #if defined(__x86_64__) || defined(__aarch64__)
                log("Linux 64-bits system detected!");
            #else
                #ifdef HANDLE_32_BITS_SYSTEMS
                    log("Linux 32-bits system detected!");
                #else
                    log("Linux 32-bits systems are not supported! Please upgrade to a 64-bit system to run this program.");
                #endif
            #endif
        #else
            fatal_error_log("Linux systems are not supported!");
        #endif
    #elif defined(_WIN32)
        #ifdef HANDLE_WINDOWS
            #if defined(_WIN64)
                log("Windows 64-bits system detected!");
            #else
                #ifndef HANDLE_32_BITS_SYSTEMS
                    log("Windows 32-bits system detected!");
                #else
                    fatal_error_log("Windows 32-bits systems are not supported! Please upgrade to a 64-bit system to run this program.");
                #endif
            #endif
        #else
            fatal_error_log("Windows systems are not supported!");
        #endif
    #else
        std::string engine_version = ENGINE_VERSION_VARIANT + "." + ENGINE_VERSION_MAJOR + "." + ENGINE_VERSION_MINOR + "." + ENGINE_VERSION_PATCH;
        std::string game_version = GAME_VERSION_VARIANT + "." + GAME_VERSION_MAJOR + "." + GAME_VERSION_MINOR + "." + GAME_VERSION_PATCH;

        fatal_error_log("Your operating system is not supported by the engine! Visit https://github.com/ArchorByte/osge to check if a support is planned.\nEngine version: " + engine_version + ".\nGame version: " + game_version + ".");
    #endif
}
