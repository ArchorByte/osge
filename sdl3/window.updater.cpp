#include "window.updater.hpp"

#include "../logs/logs.handler.hpp"
#include "../helpers/help.text.format.hpp"

#include <SDL3/SDL.h>
#include <string>

enum WindowModes
{
    WINDOWED = 0,
    BORDERLESS = 1,
    FULLSCREEN = 2
};

// Update the properties of an SDL3 window.
// Note: The refresh rate setting only affect the window in FULLSCREEN mode.
void update_sdl3_window
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
        error_log("SDL3 window update failed! The window provided (" + force_string(window) + ") is not valid!");
        return;
    }

    if (new_window_mode < WINDOWED || new_window_mode > FULLSCREEN)
    {
        error_log("Warning: The new window mode provided (" + std::to_string(new_window_mode) + ") is not valid! Defaulted to FULLSCREEN.");
        new_window_mode = FULLSCREEN;
    }

    if (new_width < 0)
    {
        error_log("SDL3 window update failed! The new window width provided (" + std::to_string(new_width) + ") is not valid!");
        return;
    }

    if (new_height < 0)
    {
        error_log("SDL3 window update failed! The new window height provided (" + std::to_string(new_height) + ") is not valid!");
        return;
    }

    if (new_refresh_rate < 1)
    {
        error_log("SDL3 window update failed! The new window refresh rate provided (" + std::to_string(new_refresh_rate) + ") is not valid!");
        return;
    }

    // Try to retrieve the ID of the targeted display.
    SDL_DisplayID display_id = SDL_GetDisplayForWindow(window);

    if (display_id == 0)
    {
        error_log("SDL3 window update failed! Failed to get the display ID of display #" + std::to_string(new_display_index) + " with the error code " + std::string(SDL_GetError()));
        return;
    }

    if (new_window_mode != FULLSCREEN)
    {
        // Update the window width and height.
        SDL_SetWindowSize(window, new_width, new_height);
    }
    else
    {
        const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display_id);

        if (mode)
        {
            SDL_DisplayMode new_mode;

            new_mode.w = new_width;
            new_mode.h = new_height;
            new_mode.refresh_rate = new_refresh_rate;

            int new_mode_apply = SDL_SetWindowFullscreenMode(window, &new_mode);

            if (new_mode_apply != 0)
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
            SDL_SetWindowBordered(window, false);
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
