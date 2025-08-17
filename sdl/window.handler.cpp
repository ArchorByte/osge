#include "window.handler.hpp"

#include "../logs/logs.handler.hpp"
#include "../config/engine.config.hpp"
#include "../utils/tool.text.format.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <string>
#include <iostream>

//////////////////////////////////////////////////////
//////////////////// ENUMERATIONS ////////////////////
//////////////////////////////////////////////////////

enum Window_Modes
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

// Create a window using SDL2.
// Warning: We assume that SDL2 has already been initialized. Normally, the engine takes care of it on startup.
SDL_Window* create_sdl_window
(
    int width,
    int height,
    int window_mode,
    std::string window_name,
    int graphic_api
)
{
    log("Creating an SDL2 window..");

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

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fatal_error_log("SDL2 initialization failed: " + std::string(SDL_GetError()));
    }

    // Window flags
    Uint32 flags = SDL_WINDOW_HIDDEN; // Start hidden until we configure
    if (graphic_api == VULKAN) flags |= SDL_WINDOW_VULKAN;
    if (graphic_api == OPENGL) flags |= SDL_WINDOW_OPENGL;

    switch (window_mode)
    {
        case WINDOWED:
            flags |= SDL_WINDOW_RESIZABLE;
            break;

        case BORDERLESS:
            flags |= SDL_WINDOW_BORDERLESS;
            break;

        case FULLSCREEN:
            flags |= SDL_WINDOW_FULLSCREEN;
            break;
    }

    // Debug suffix
    #ifdef DEBUG_MODE
        window_name += " - Debug Mode";
    #endif

    SDL_Window* window = SDL_CreateWindow(
        window_name.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        flags
    );

    if (!window)
    {
        fatal_error_log("SDL2 window creation failed with error code " + std::string(SDL_GetError()) + "!");
    }

    SDL_ShowWindow(window);
    log("SDL2 window " + force_string(window) + " created successfully!");

    return window;
}

// Destroy a SDL2 window.
void destroy_sdl_window
(
    SDL_Window* window
)
{
    log("Destroying the " + force_string(window) + " SDL2 window..");

    if (!window)
    {
        error_log("SDL2 window destruction failed! The window provided (" + force_string(window) + ") is not valid!");
        return;
    }

    SDL_DestroyWindow(window);
    log("SDL2 window destroyed successfully!");

    SDL_Quit();
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

SDL2_Window::SDL2_Window
(
    int width,
    int height,
    int window_mode,
    std::string window_name,
    int graphic_api
)
{
    window = create_sdl_window(width, height, window_mode, window_name, graphic_api);
}

SDL2_Window::~SDL2_Window()
{
    destroy_sdl_window(window);
}

SDL_Window* SDL2_Window::get() const
{
    return window;
}
