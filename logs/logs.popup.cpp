#include "logs.popup.hpp"

#include "../config/engine.config.hpp"
#include "../config/game.config.hpp"

#if defined(_WIN64)
    #include <windows.h>
#endif

#if defined(__linux__)
    #include <SDL3/SDL.h>
#endif

#include "string"

// Open an error pop-up when the process crashed.
void open_crash_popup
(
    const std::string &message
)
{
    #ifndef USE_CRASH_ERROR_MESSAGE_BOXES
        return;
    #endif

    // Some text format.
    std::string popup_title = std::string(GAME_TITLE) + " has crashed!";
    std::string full_message;

    // In debug mode, we give the custom error message.
    // However, in release mode, we give a basic message.
    #ifdef DEBUG_MODE
        full_message = "Error message: " + message;
    #else
        full_message = "The game had a critical error, a restart is required! Sorry for the inconvenience.";
    #endif

    #if defined(_WIN64)
        // Simple Windows message box using Windows API.
        MessageBoxA
        (
            nullptr,
            full_message.c_str(),
            popup_title.c_str(),
            MB_ICONERROR | MB_OK
        );
    #elif defined(__linux__)
        // Simple message box using SDL3.
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            popup_title.c_str(),
            full_message.c_str(),
            nullptr
        );
    #endif
}
