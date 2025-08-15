#ifndef ENGINE_CONFIG_HPP
#define ENGINE_CONFIG_HPP

// ONLY USE DEBUG MODE IF YOU ARE TESTING YOUR PRODUCT.
// COMMENT OR REMOVE THAT LINE IF YOU BUILD YOUR PRODUCT FOR THE FINAL USERS.
// When the DEBUG_MODE flag exists, OSGE enables:
// - A console with logs in it.
// - The Khronos validation layers (Vulkan SDK required).
#define DEBUG_MODE

// Display a message box to the user when the engine or the game crashes.
// Comment or remove that line if you don't want to use it.
// Note: The message displayed depends on the debug mode state.
//       If the debug mode is active, we give the full error.
//       If the debug mode is NOT active, we just inform the user that it crashed and a restart is needed.
#define USE_CRASH_ERROR_MESSAGE_BOXES

// Comment or remove that line if you don't want the logs file.
// Note 1: The debug mode state doesn't influence the logs file. That means this file can exist even if we are not using the debug mode.
// Note 2: We DO NOT RECOMMEND to enable the logs file for the final users because some sensitive data such as memory addresses will be written in it!
#define ALLOW_LOGS_FILE
#define LOGS_FILE_NAME "osge.logs"

// Comment or remove that line if you want to disallow 32-bits system to run your product.
// WARNING: Every operating system supported will be affected by this flag.
#define HANDLE_32_BITS_SYSTEMS

// Comment or remove the lines below if you want to disable support for one/some specific operating systems.
// Note: These options don't make any distinction between 64-bits and 32-bits systems. Only the flag above does.
#define HANDLE_LINUX
#define HANDLE_WINDOWS

#endif
