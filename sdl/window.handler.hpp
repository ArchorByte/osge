#include <SDL2/SDL.h>
#include <string>

#ifndef SDL2_WINDOW_HPP
#define SDL2_WINDOW_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a window using SDL2.
SDL_Window* create_sdl_window
(
    int width,
    int height,
    int window_mode,
    std::string window_name,
    int graphic_api
);

// Destroy an SDL2 window.
void destroy_sdl_window
(
    SDL_Window* window
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class SDL2_Window
{

public:
    // Constructor.
    SDL2_Window
    (
        int width,
        int height,
        int window_mode,
        std::string window_name,
        int graphic_api
    );

    // Destructor.
    ~SDL2_Window();

    SDL_Window* get() const;

    // Prevent data duplication.
    SDL2_Window(const SDL2_Window&) = delete;
    SDL2_Window& operator=(const SDL2_Window&) = delete;

private:
    SDL_Window* window;

};

#endif
