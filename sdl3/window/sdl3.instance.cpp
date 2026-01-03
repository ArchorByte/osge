#include "sdl3.instance.hpp"

#include "../logs/logs.handler.hpp"

#include <SDL3/SDL.h>

// Initialize SDL3.
void start_sdl3_instance()
{
    log("Initializing SDL3..");
    const int initialization = SDL_Init(SDL_INIT_VIDEO);

    if (initialization == 0)
    {
        fatal_error_log("Failed to initialize SDL3! Error: " + std::string(SDL_GetError()));
    }

    log("SDL3 successfully initialized!");
}

// Destroy an SDL3 instance.
void destroy_sdl3_instance()
{
    log("Destroying the SDL3 instance..");
    SDL_Quit();
    log("SDL3 instance destroyed successfully!");
}
