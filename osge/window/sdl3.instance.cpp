#include "window.hpp"

#include "../utils/utils.hpp"
#include "libraries/sdl/SDL3/SDL.h"

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Initialize an SDL3 instance.
    Note: You should use the pre-made class to handle these objects rather than directly using this function for memory safety reasons.

    Tasks:
        1) Try to initialize SDL3.
        2) Verify initialization output.

    Parameters:
        No parameters.

    Returns:
        No object returned.
*/
void Window::create_sdl3_instance()
{
    Utils::Logs::log("Initializing SDL3.. ", false);
    const int initialization = SDL_Init(SDL_INIT_VIDEO);

    if (initialization == 0)
        Utils::Logs::crash_log("Failed! An error occurred -> " + std::string(SDL_GetError()));

    Utils::Logs::log("Done!", true);
}



/*
    Destroy an SDL3 instance.

    Tasks:
        1) Close SDL3.

    Parameters:
        No parameters.

    Returns:
        No object returned.
*/
void Window::destroy_sdl3_instance()
{
    Utils::Logs::log("Destroying SDL3 instance.. ", false);
    SDL_Quit();
    Utils::Logs::log("Done!", true);
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Window::sdl3_instance_handler::sdl3_instance_handler()
{
    create_sdl3_instance();
}

Window::sdl3_instance_handler::~sdl3_instance_handler()
{
    destroy_sdl3_instance();
}
