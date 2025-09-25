#ifdef ENGINE_CONFIG_HPP
#    undef ENGINE_CONFIG_HPP
#endif

#define ENGINE_CONFIG_HPP

namespace EngineConfig
{

// ONLY USE DEBUG MODE IF YOU ARE TESTING YOUR PRODUCT.
// SET TO FALSE THAT FLAG IF YOU BUILD YOUR PRODUCT FOR THE FINAL USERS.
// When DEBUG_MODE is on, OSGE enables:
// - A console with logs in it.
// - The Khronos validation layers (Vulkan SDK required).
constexpr bool DEBUG_MODE = true;

// Display a message box to the user when the program crashes.
// Set to false that flag if you don't want to use it.
// Note: The message displayed depends on the debug mode state.
//       - If the debug mode is active, we give the full error.
//       - If the debug mode is NOT active, we just inform the user that it crashed and a restart is needed.
constexpr bool USE_CRASH_ERROR_MESSAGE_BOXES = true;

// Set to false that flag if you don't want the logs file.
// Note 1: The debug mode state doesn't influence the logs file. That means this file can exist even if we are not using the debug mode.
// Note 2: We DO NOT RECOMMEND to enable the logs file for the final users because it writes the debug mode outputs into a file!
constexpr bool ENABLE_LOGS_FILE = true;
constexpr char* LOGS_FILE_NAME = "osge.logs";

// Set to false the flags below if you want to disable support for one/some specific operating systems.
constexpr bool HANDLE_LINUX_SYSTEMS = true;
constexpr bool HANDLE_WINDOWS_SYSTEMS = true;

}
