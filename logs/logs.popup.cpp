#include "logs.popup.hpp"

#include "../config/engine.config.hpp"
#include "../config/game.config.hpp"

#include <SDL3/SDL.h>
#include <string>

// Open an error pop-up when the process crashed.
void open_crash_popup
(
    const std::string &message
)
{
    if constexpr (!EngineConfig::USE_CRASH_ERROR_MESSAGE_BOXES)
        return;

    const std::string popup_title = std::string(GameConfig::GAME_TITLE) + " has crashed!";
    std::string full_message;

    // In debug mode, we give the error that made the app crash.
    // However, in release mode, we give a basic message.
    if constexpr (EngineConfig::DEBUG_MODE)
        full_message = "Error message: " + message;
    else
        full_message = "The game had a critical error and crashed. To continue, a restart is required. Sorry for the inconvenience.";

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, popup_title.c_str(), full_message.c_str(), nullptr);
}
