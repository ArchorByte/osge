#include "window.hpp"

#include "../utils/utils.hpp"
#include "libraries/sdl/SDL3/SDL.h"

#include <string>

/*
    Make and open a message box with SDL3.

    Tasks:
        1) Verify function parameters.
        2) Create the message box.

    Parameters:
        - icon    / SDL_MessageBoxFlags / SDL3 icon to use.
        - message / string              / Message to display in the message box.
        - title   / string              / Title of the message box.

    Returns:
        No object returned.
*/
void Window::open_message_box
(
    const SDL_MessageBoxFlags &icon,
    const std::string         &message,
    std::string               title
)
{
    if (message.size() < 1)
    {
        Utils::Logs::log("Warning: Failed to open a message box, empty message provided.", true);
        return;
    }

    if (title.size() < 1)
    {
        Utils::Logs::log("Warning: Invalid message box title provided!", true);
        title = "New message box";
    }

    SDL_ShowSimpleMessageBox(icon, title.c_str(), message.c_str(), nullptr);
}
