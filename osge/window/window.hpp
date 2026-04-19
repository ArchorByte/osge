#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "libraries/sdl/SDL3/SDL.h"
#include <string>

//////////////////////////////////////////////////////
//////////////////// Enumerations ////////////////////
//////////////////////////////////////////////////////

enum WindowModes
{
    WINDOWED = 0,
    BORDERLESS = 1,
    FULLSCREEN = 2
};

///////////////////////////////////////////////////
//////////////////// Namespace ////////////////////
///////////////////////////////////////////////////

namespace Window
{
    ///////////////////////////////////////////////////
    //////////////////// Functions ////////////////////
    ///////////////////////////////////////////////////

    ///////////////////////////
    ///// game.window.cpp /////
    ///////////////////////////

    SDL_Window* create_sdl3_window
    (
        int         &height,
        int         &graphic_api,
        const int   &width,
        int         window_mode,
        std::string window_name
    );

    void update_sdl3_window
    (
        const int   &new_display_index,
        int         new_height,
        const int   &new_refresh_rate,
        const int   &new_width,
        int         new_window_mode,
        SDL_Window* window
    );

    void destroy_sdl3_window
    (
        SDL_Window* window
    );

    /////////////////////////////
    ///// message.boxes.cpp /////
    /////////////////////////////

    void open_message_box
    (
        const SDL_MessageBoxFlags &icon,
        const std::string         &message,
        std::string               title
    );

    /////////////////////////////
    ///// sdl3.instance.cpp /////
    /////////////////////////////

    void create_sdl3_instance();

    void destroy_sdl3_instance();

    ///////////////////////////////////////////////
    //////////////////// Class ////////////////////
    ///////////////////////////////////////////////

    ///////////////////////////
    ///// game.window.cpp /////
    ///////////////////////////

    class sdl3_game_window_handler
    {
        public:
            sdl3_game_window_handler
            (
                int         &graphic_api,
                int         &height,
                const int   &width,
                int         window_mode,
                std::string window_name
            );

            ~sdl3_game_window_handler();
            SDL_Window* get() const;

            // Prevent data duplication.
            sdl3_game_window_handler(const sdl3_game_window_handler&) = delete;
            sdl3_game_window_handler &operator = (const sdl3_game_window_handler&) = delete;
        private:
            SDL_Window* window;
    };

    /////////////////////////////
    ///// sdl3.instance.cpp /////
    /////////////////////////////

    class sdl3_instance_handler
    {
        public:
            sdl3_instance_handler();
            ~sdl3_instance_handler();

            // Prevent data duplication.
            sdl3_instance_handler(const sdl3_instance_handler&) = delete;
            sdl3_instance_handler &operator = (const sdl3_instance_handler&) = delete;
    };
}

#endif
