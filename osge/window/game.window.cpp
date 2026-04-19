#include "window.hpp"

#include "../osge.hpp"
#include "../config/engine.config.hpp"
#include "../utils/utils.hpp"
#include "libraries/sdl/SDL3/SDL.h"
#include "libraries/sdl/SDL3/SDL_vulkan.h"

#include <string>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a window using SDL3.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.
    Warning: We assume that SDL3 has been already initialized.

    Tasks:
        1) Verify function parameters.
        2) Prepare window flags.
        3) Create window.
        4) Display window.

    Parameters:
        - graphic_api / int    / Graphics API that will be used.
        - height      / int    / Height to use for the window resolution.
        - width       / int    / Width to use for the window resolution.
        - window_mode / int    / Defines whether the window should be windowed, borderless or fullscreen.
        - window_name / string / Name that will be given to the window.

    Returns:
        The created SDL3 window.
*/
SDL_Window* Window::create_sdl3_window
(
    int         &graphic_api,
    int         &height,
    const int   &width,
    int         window_mode,
    std::string window_name
)
{
    Utils::Logs::log("Creating SDL3 window.. ", false);

    if (window_mode < WINDOWED || window_mode > FULLSCREEN)
    {
        Utils::Logs::log("\nWarning: Invalid window mode provided, defaulted to FULLSCREEN mode.", true);
        window_mode = FULLSCREEN;
    }

    if (graphic_api < VULKAN || graphic_api > OPENGL)
    {
        Utils::Logs::log("\nWarning: Invalid graphic API provided, defaulted to Vulkan.", true);
        graphic_api = VULKAN;
    }

    const float ratio = static_cast<float>(width) / height;

    if (!abs(ratio - (16.0f / 9.0f)) < 0.01f)
    {
        const int new_height = width * 9 / 16;
        Utils::Logs::log("\nWarning: Invalid resolution provided, defaulted to " + std::to_string(width) + "x" + std::to_string(new_height) + ".", true);
        height = new_height;
    }

    Uint32 flags = SDL_WINDOW_HIDDEN;

    if (graphic_api == VULKAN)
        flags |= SDL_WINDOW_VULKAN;
    else if (graphic_api == OPENGL)
        flags |= SDL_WINDOW_OPENGL;

    switch (window_mode)
    {
        case BORDERLESS:
            flags |= SDL_WINDOW_BORDERLESS;
            break;
        case FULLSCREEN:
            flags |= SDL_WINDOW_FULLSCREEN;
            break;
    }

    if constexpr (EngineConfig::DEBUG_MODE)
        window_name += " - Debug Mode";

    SDL_Window* window = SDL_CreateWindow(window_name.c_str(), width, height, flags);

    if (!window)
        Utils::Logs::crash_log("Failed! Error code " + std::string(SDL_GetError()));

    SDL_ShowWindow(window);
    Utils::Logs::log("Done! Memory address -> " + Utils::Text::get_memory_address(window) + ".", true);

    return window;
}



/*
    Update the properties of an SDL3 window.

    Tasks:
        1) Verify function parameters.
        2) Set new window position depending on the new display index.
        3) Get new display ID.
        4) Modify window properties.
            - If not fullscreen mode, we make a simple modification.
            - If fullscreen mode, we define a new mode with resolution and refresh rate and try to apply it.
        5) Set new flags and try to apply them.

    Parameters:
        - new_display_index / int         / Index of the new screen to use.
        - new_height        / int         / New height of the window.
        - new_refresh_rate  / int         / New refresh rate of the window.
        - new_width         / int         / New width of the window.
        - new_window_mode   / int         / Defines whether the window should be windowed, borderless or fullscreen.
        - window            / SDL_Window* / SDL3 window to modify.

    Returns:
        No object returned.
*/
void Window::update_sdl3_window
(
    const int   &new_display_index,
    int         new_height,
    const int   &new_refresh_rate,
    const int   &new_width,
    int         new_window_mode,
    SDL_Window* window
)
{
    Utils::Logs::log("Updating " + Utils::Text::get_memory_address(window) + " SDL3 window properties.. ", false);

    if (new_refresh_rate < 1)
    {
        Utils::Logs::log("Failed! New window refresh rate invalid -> " + std::to_string(new_refresh_rate) + ".", true);
        return;
    }

    if (new_width < 0)
    {
        Utils::Logs::log("Failed! New window width invalid -> " + std::to_string(new_width) + ".", true);
        return;
    }

    const float display_ratio = static_cast<float>(new_width) / new_height;

    if (!abs(display_ratio - (16.0f / 9.0f)) < 0.01f)
    {
        new_height = new_width * 9 / 16;
        Utils::Logs::log("\nWarning: Invalid resolution provided, defaulted to " + std::to_string(new_width) + "x" + std::to_string(new_height) + ".", true);
    }

    if (new_window_mode < WINDOWED || new_window_mode > FULLSCREEN)
    {
        Utils::Logs::log("\nWarning: New window mode invalid -> " + std::to_string(new_window_mode) + ". Defaulted to FULLSCREEN.", true);
        new_window_mode = FULLSCREEN;
    }

    if (!window)
    {
        Utils::Logs::log("Failed! Window invalid.", true);
        return;
    }

    SDL_SetWindowPosition
    (
        window,
        SDL_WINDOWPOS_CENTERED_DISPLAY(new_display_index),
        SDL_WINDOWPOS_CENTERED_DISPLAY(new_display_index)
    );

    const SDL_DisplayID display_id = SDL_GetDisplayForWindow(window);

    if (display_id == 0)
    {
        Utils::Logs::log("Failed! Display ID query returned error code -> " + std::string(SDL_GetError()), true);
        return;
    }

    if (new_window_mode != FULLSCREEN)
        SDL_SetWindowSize(window, new_width, new_height);
    else
    {
        const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display_id);

        if (!mode)
        {
            Utils::Logs::log("Failed! Display mode query returned error code -> " + std::string(SDL_GetError()), true);
            return;
        }

        SDL_DisplayMode new_mode;

        new_mode.w = new_width;
        new_mode.h = new_height;
        new_mode.refresh_rate = new_refresh_rate;

        const int apply = SDL_SetWindowFullscreenMode(window, &new_mode);

        if (apply != 0)
        {
            Utils::Logs::log("Failed! Application of display mode returned error code -> " + std::string(SDL_GetError()), true);
            return;
        }
    }

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

    const int modification = SDL_SetWindowFullscreen(window, flags);

    if (modification != 0)
        Utils::Logs::log("Failed! Flags modifications returned error code -> " + std::string(SDL_GetError()), true);

    Utils::Logs::log("Done!", true);
}



/*
    Destroy a SDL3 window.

    Tasks:
        1) Verify function parameters.
        2) Destroy window.

    Parameters:
        - window / SDL_Window / SDL3 window to destroy.

    Returns:
        No object returned.
*/
void Window::destroy_sdl3_window
(
    SDL_Window* window
)
{
    Utils::Logs::log("Destroying " + Utils::Text::get_memory_address(window) + " SDL3 window.. ", false);

    if (!window)
    {
        Utils::Logs::log("Failed! Invalid window.", true);
        return;
    }

    SDL_DestroyWindow(window);
    Utils::Logs::log("Done!", true);
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Window::sdl3_game_window_handler::sdl3_game_window_handler
(
    int         &graphic_api,
    int         &height,
    const int   &width,
    int         window_mode,
    std::string window_name
)
{
    window = create_sdl3_window(graphic_api, height, width, window_mode, window_name);
}

Window::sdl3_game_window_handler::~sdl3_game_window_handler()
{
    destroy_sdl3_window(window);
}

SDL_Window* Window::sdl3_game_window_handler::get() const
{
    return window;
}
