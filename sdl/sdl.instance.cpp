#include "sdl.instance.hpp"

#include "../logs/logs.handler.hpp"

#include <SDL2/SDL.h>

// Initialize SDL2.
void start_sdl_instance()
{
    log("Initializing SDL2..");
    int initialization = SDL_Init(SDL_INIT_VIDEO);

    if (initialization < 0)
    {
        fatal_error_log("Failed to initialize SDL2! Error: " + std::string(SDL_GetError()));
    }

    log("SDL2 successfully initialized!");
}

// Destroy an SDL2 instance.
void destroy_sdl_instance()
{
    log("Destroying the SDL2 instance..");
    SDL_Quit();
    log("SDL2 instance destroyed successfully!");
}
