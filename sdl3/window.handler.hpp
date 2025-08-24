#include <SDL3/SDL.h>
#include <string>

#ifndef SDL3_WINDOW_HPP
#define SDL3_WINDOW_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

SDL_Window* create_sdl3_window
(
    int width,
    int height,
    int window_mode,
    std::string window_name,
    int graphic_api
);

void destroy_sdl3_window
(
    SDL_Window* window
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class SDL3_Window
{

public:
    // Constructor.
    SDL3_Window
    (
        int width,
        int height,
        int window_mode,
        std::string window_name,
        int graphic_api
    );

    // Destructor.
    ~SDL3_Window();

    SDL_Window* get() const;

    // Prevent data duplication.
    SDL3_Window(const SDL3_Window&) = delete;
    SDL3_Window& operator=(const SDL3_Window&) = delete;

private:
    // We declare the members of the class to store.
    SDL_Window* window;

};

#endif
