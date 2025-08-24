#include "env.resolutions.hpp"

#include "../logs/logs.handler.hpp"
#include "../utils/tool.text.format.hpp"

#include <SDL3/SDL.h>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

// Get and return the resolution of a display using GLFW.
// Warning: We assume that GLFW has already been initialized! Normally, the engine takes care of that by default on startup.
std::pair<int, int> get_display_resolution
(
    int display_index
)
{
    log("Detecting the display resolution..");
    std::pair<int, int> output;

    // Retrieve the video data of the display.
    const SDL_DisplayMode* display_mode = SDL_GetCurrentDisplayMode(display_index);

    if (!display_mode)
    {
        fatal_error_log("Display resolution detection failed! Failed to retrieve the video mode with the error code " + std::string(SDL_GetError()) + "!");
    }

    // Get the width and height of the display.
    int width = display_mode -> w;
    int height = display_mode -> h;

    log("Detected resolution: " + std::to_string(width) + "x" + std::to_string(height) + "!");
    return { width, height };
}

// Get and return every "allowed" resolution for the game, depending on the display resolution.
std::vector<std::string> get_allowed_game_resolutions
(
    const std::pair<int, int> &display_resolution
)
{
    log("Detecting allowed game resolutions..");
    std::vector<std::string> output;

    int display_width = display_resolution.first;
    int display_height = display_resolution.second;

    // Calculate the display ratio (width/height).
    float display_ratio = static_cast<float>(display_width) / display_height;

    // We allow directly the current display resolution if it's already a 16/9 resolution.
    // Otherwise, we calculate the valid height for the display width to obtain a 16/9 resolution.
    if (abs(display_ratio - (16.0f / 9.0f)) < 0.01f) output.push_back(std::to_string(display_width) + "x" + std::to_string(display_height));
    else output.push_back(std::to_string(display_width) + "x" + std::to_string(display_width * 9 / 16));

    // Now we try to find out what common 16/9 resolutions we can use on this display.
    if (display_width >= 640 && display_height >= 360) output.push_back("640x360");
    if (display_width >= 854 && display_height >= 480) output.push_back("854x480");
    if (display_width >= 1280 && display_height >= 720) output.push_back("1280x720");
    if (display_width >= 1600 && display_height >= 900) output.push_back("1600x900");
    if (display_width >= 1920 && display_height >= 1080) output.push_back("1920x1080");
    if (display_width >= 2560 && display_height >= 1440) output.push_back("2560x1440");
    if (display_width >= 3840 && display_height >= 2160) output.push_back("3840x2160");

    log(std::to_string(output.size()) + " game resolutions are allowed!");
    return output;
}

// Return the game resolution that we are going to use at start.
std::pair<int, int> select_game_resolution
(
    std::map<std::string, std::string> config,
    const std::pair<int, int> &display_resolution,
    const std::vector<std::string> &allowed_game_resolutions
)
{
    log("Calculating game resolution at start..");

    // We make string chains with the config and display resolutions.
    // Format: [width]x[height] (without the square brackets).
    std::string string_config_resolution = config["WINDOW_WIDTH"] + "x" + config["WINDOW_HEIGHT"];
    std::string string_display_resolution = std::to_string(display_resolution.first) + "x" + std::to_string(display_resolution.second);

    // We try to find the config file resolution in the allowed resolutions list.
    if (find(allowed_game_resolutions.begin(), allowed_game_resolutions.end(), string_config_resolution) != allowed_game_resolutions.end())
    {
        // Read the values in the config map.
        int width = stoi(config["WINDOW_WIDTH"]);
        int height = stoi(config["WINDOW_HEIGHT"]);

        log("The game resolution at start will be " + std::to_string(width) + "x" + std::to_string(height) + "! Selected from config file.");
        return { width, height };
    }

    error_log("The resolution in the config file (" + string_config_resolution + ") is not a 16/9 resolution! Switching to display resolution..");

    // We try to find the display resolution in the allowed resolutions list.
    if (find(allowed_game_resolutions.begin(), allowed_game_resolutions.end(), string_display_resolution) != allowed_game_resolutions.end())
    {
        // Retrieve the display resolution data.
        int width = display_resolution.first;
        int height = display_resolution.second;

        log("The game resolution at start will be " + std::to_string(width) + "x" + std::to_string(height) + "! Selected from display resolution.");
        return { width, height };
    }

    error_log("The display resolution (" + string_display_resolution + ") is not a 16/9 resolution! Switching to allowed game resolutions..");

    int biggest_width = 0;
    int biggest_height = 0;

    // Analyze each resolution in the allowed resolutions list.
    // We will take the highest resolution.
    for (const std::string resolution : allowed_game_resolutions)
    {
        // Detect the x position in the resolution string.
        size_t x_position = resolution.find("x");
        if (x_position == std::string::npos) continue; // We skip the resolution if no "x" was found.

        // Scrape the width and height values using the x position.
        int width = stoi(resolution.substr(0, x_position));
        int height = stoi(resolution.substr(x_position + 1));

        if (!width || !height) continue;

        if (biggest_width < width)
        {
            biggest_width = width;
            biggest_height = height;
        }
    }

    if (biggest_width == 0 || biggest_height == 0)
    {
        fatal_error_log("Resolution selection failed! No 16/9 resolution is available for this device! Please verify your video settings.");
    }

    log("The game resolution at start will be " + std::to_string(biggest_width) + "x" + std::to_string(biggest_height) + "! Selected from the highest allowed game resolution.");
    return { biggest_width, biggest_height };
}
