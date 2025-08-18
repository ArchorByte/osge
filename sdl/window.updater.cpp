#include "window.updater.hpp"

#include "../logs/logs.handler.hpp"
#include "../utils/tool.text.format.hpp"

#include <SDL2/SDL.h>
#include <string>

enum WindowModes
{
    WINDOWED = 0,
    BORDERLESS = 1,
    FULLSCREEN = 2
};

// Update the properties of a SDL window.
// Note: The refresh rate setting only affect the window in FULLSCREEN mode.
void update_sdl_window
(
    SDL_Window* window,
    int new_window_mode,
    int new_width,
    int new_height,
    int new_refresh_rate,
    int new_display_index
)
{
    if (!window)
    {
        error_log("SDL2 window update failed! The window provided (" + force_string(window) + ") is not valid!");
        return;
    }

    if (new_window_mode < WINDOWED || new_window_mode > FULLSCREEN)
    {
        error_log("Warning: The new window mode provided (" + std::to_string(new_window_mode) + ") is not valid! Defaulted to FULLSCREEN.");
        new_window_mode = FULLSCREEN;
    }

    if (new_width < 0)
    {
        error_log("SDL2 window update failed! The new window width provided (" + std::to_string(new_width) + ") is not valid!");
        return;
    }

    if (new_height < 0)
    {
        error_log("SDL2 window update failed! The new window height provided (" + std::to_string(new_height) + ") is not valid!");
        return;
    }

    if (new_refresh_rate < 1)
    {
        error_log("SDL2 window update failed! The new window refresh rate provided (" + std::to_string(new_refresh_rate) + ") is not valid!");
        return;
    }

    if (new_display_index < 0 || new_display_index >= SDL_GetNumVideoDisplays())
    {
        error_log("SDL2 window update failed! The new display index provided (" + std::to_string(new_display_index) + ") is not valid!");
        return;
    }

    if (new_window_mode != FULLSCREEN)
    {
        // Update the window width and height.
        SDL_SetWindowSize(window, new_width, new_height);
    }
    else
    {
        SDL_DisplayMode mode;
        int query = SDL_GetCurrentDisplayMode(new_display_index, &mode);

        if (query == 0)
        {
            mode.w = new_width;
            mode.h = new_height;
            mode.refresh_rate = new_refresh_rate;

            int settings_application = SDL_SetWindowDisplayMode(window, &mode);

            if (settings_application != 0)
            {
                error_log("The new FULLSCREEN settings application (" + std::to_string(new_width) + "x" + std::to_string(new_height) + "@" + std::to_string(new_refresh_rate) + ") failed with error code " + std::string(SDL_GetError()) + "!");
            }
        }
        else error_log("The current display mode query failed with error code " + std::string(SDL_GetError()) + "!");
    }

    // Move the window to the center of the targeted display.
    // The engine always place the window at the center.
    SDL_SetWindowPosition
    (
        window,
        SDL_WINDOWPOS_CENTERED_DISPLAY(new_display_index),
        SDL_WINDOWPOS_CENTERED_DISPLAY(new_display_index)
    );

    // We let the flags to 0 for the WINDOWED mode.
    Uint32 flags = 0;

    switch (new_window_mode)
    {
        case BORDERLESS:
            SDL_SetWindowBordered(window, SDL_FALSE);
            break;

        case FULLSCREEN:
            flags = SDL_WINDOW_FULLSCREEN;
            break;
    }

    int flags_application = SDL_SetWindowFullscreen(window, flags);

    if (flags_application != 0)
    {
        error_log("The flags application (" + std::to_string(flags) + ") failed with error code " + std::string(SDL_GetError()) + "!");
    }
}
