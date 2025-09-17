#include "window.handler.hpp"

#include "../logs/logs.handler.hpp"
#include "../config/engine.config.hpp"
#include "../helpers/help.text.format.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <string>
#include <iostream>

//////////////////////////////////////////////////////
//////////////////// ENUMERATIONS ////////////////////
//////////////////////////////////////////////////////

enum WindowModes
{
    WINDOWED = 0,
    BORDERLESS = 1,
    FULLSCREEN = 2
};

enum GraphicsAPI
{
    VULKAN = 0,
    OPENGL = 1
};

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a window using SDL3.
// Warning: We assume that SDL3 has already been initialized. Normally, the engine takes care of it on startup.
SDL_Window* create_sdl3_window
(
    int width,
    int height,
    int window_mode,
    std::string window_name,
    int graphic_api
)
{
    log("Creating an SDL3 window..");

    if (window_mode < WINDOWED || window_mode > FULLSCREEN)
    {
        error_log("Warning: Invalid window mode provided! Defaulted to FULLSCREEN mode.");
        window_mode = FULLSCREEN;
    }

    if (graphic_api < VULKAN || graphic_api > OPENGL)
    {
        error_log("Warning: Invalid graphic API provided! Defaulted to Vulkan.");
        graphic_api = VULKAN;
    }

    int initialization = SDL_Init(SDL_INIT_VIDEO);

    if (initialization < 0)
    {
        fatal_error_log("SDL3 initialization failed: " + std::string(SDL_GetError()));
    }

    // We hide the window.
    Uint32 flags = SDL_WINDOW_HIDDEN;

    if (graphic_api == VULKAN) flags |= SDL_WINDOW_VULKAN;
    if (graphic_api == OPENGL) flags |= SDL_WINDOW_OPENGL;

    // We apply specific flags for the BORDERLESS and FULLSCREEN modes.
    switch (window_mode)
    {
        case BORDERLESS:
            flags |= SDL_WINDOW_BORDERLESS;
            break;

        case FULLSCREEN:
            flags |= SDL_WINDOW_FULLSCREEN;
            break;
    }

    // Add a "Debug Mode" suffix to the window name if we're running in debug mode.
    #ifdef DEBUG_MODE
        window_name += " - Debug Mode";
    #endif

    // Create the window itself.
    SDL_Window* window = SDL_CreateWindow
    (
        window_name.c_str(),
        width,
        height,
        flags
    );

    if (!window)
    {
        fatal_error_log("SDL3 window creation failed with error code " + std::string(SDL_GetError()) + "!");
    }

    SDL_ShowWindow(window); // We finally show the window.
    log("SDL3 window " + force_string(window) + " created successfully!");

    return window;
}

// Destroy a SDL3 window.
void destroy_sdl3_window
(
    SDL_Window* window
)
{
    log("Destroying the " + force_string(window) + " SDL3 window..");

    if (!window)
    {
        error_log("SDL3 window destruction failed! The window provided (" + force_string(window) + ") is not valid!");
        return;
    }

    SDL_DestroyWindow(window);
    log("SDL3 window destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
SDL3_Window::SDL3_Window
(
    int width,
    int height,
    int window_mode,
    std::string window_name,
    int graphic_api
)
{
    window = create_sdl3_window(width, height, window_mode, window_name, graphic_api);
}

// Destructor.
SDL3_Window::~SDL3_Window()
{
    destroy_sdl3_window(window);
}

SDL_Window* SDL3_Window::get() const
{
    return window;
}
