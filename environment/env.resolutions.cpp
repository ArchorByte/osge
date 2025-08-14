#include "resolutions.hpp"

#include "../logs/terminal.hpp"
#include "../tools/text_format.hpp"

#include <GLFW/glfw3.h>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

// Get and return the resolution of a monitor using GLFW.
// Warning: We assume that GLFW has already been initialized! Normally, the engine takes care of that by default on startup.
std::pair<int, int> get_monitor_resolution
(
    GLFWmonitor* monitor
)
{
    if (!monitor)
    {
        fatal_error_log("Monitor resolution detection failed! The monitor provided (" + force_string(monitor) + ") is not valid!");
    }

    log("Detecting the monitor resolution..");
    std::pair<int, int> output;

    // Retrieve the video data of the monitor.
    const GLFWvidmode* video_mode = glfwGetVideoMode(monitor);

    if (!video_mode)
    {
        fatal_error_log("Monitor resolution detection failed! Failed to retrieve the video mode!");
    }

    // Get the width and height of the monitor.
    int width = video_mode -> width;
    int height = video_mode -> height;

    log("Detected resolution: " + std::to_string(width) + "x" + std::to_string(height) + "!");
    return { width, height };
}

// Get and return every "allowed" resolution for the game, depending on the monitor resolution.
std::vector<std::string> get_allowed_game_resolutions
(
    const std::pair<int, int> &monitor_resolution
)
{
    log("Detecting allowed game resolutions..");
    std::vector<std::string> output;

    int monitor_width = monitor_resolution.first;
    int monitor_height = monitor_resolution.second;

    // Calculate the monitor ratio (width/height).
    float monitor_ratio = static_cast<float>(monitor_width) / monitor_height; 

    // We allow directly the current monitor resolution if it's already a 16/9 resolution.
    // Otherwise, we calculate the valid height for the monitor width to obtain a 16/9 resolution.
    if (abs(monitor_ratio - (16.0f / 9.0f)) < 0.01f) output.push_back(std::to_string(monitor_width) + "x" + std::to_string(monitor_height));
    else output.push_back(std::to_string(monitor_width) + "x" + std::to_string(monitor_width * 9 / 16));

    // Now we try to find out what common 16/9 resolutions we can use on this monitor.
    if (monitor_width >= 640 && monitor_height >= 360) output.push_back("640x360");
    if (monitor_width >= 854 && monitor_height >= 480) output.push_back("854x480");
    if (monitor_width >= 1280 && monitor_height >= 720) output.push_back("1280x720");
    if (monitor_width >= 1600 && monitor_height >= 900) output.push_back("1600x900");
    if (monitor_width >= 1920 && monitor_height >= 1080) output.push_back("1920x1080");
    if (monitor_width >= 2560 && monitor_height >= 1440) output.push_back("2560x1440");
    if (monitor_width >= 3840 && monitor_height >= 2160) output.push_back("3840x2160");

    log(std::to_string(output.size()) + " game resolutions are allowed!");
    return output;
}

// Return the game resolution that we are going to use at start.
std::pair<int, int> select_game_resolution
(
    std::map<std::string, std::string> config,
    const std::pair<int, int> &monitor_resolution,
    const std::vector<std::string> &allowed_game_resolutions
)
{
    log("Calculating game resolution at start..");

    // We make string chains with the config and monitor resolutions.
    // Format: [width]x[height] (without the square brackets).
    std::string string_config_resolution = config["WINDOW_WIDTH"] + "x" + config["WINDOW_HEIGHT"];
    std::string string_monitor_resolution = std::to_string(monitor_resolution.first) + "x" + std::to_string(monitor_resolution.second);

    // We try to find the config file resolution in the allowed resolutions list.
    if (find(allowed_game_resolutions.begin(), allowed_game_resolutions.end(), string_config_resolution) != allowed_game_resolutions.end())
    {
        // Read the values in the config map.
        int width = stoi(config["WINDOW_WIDTH"]);
        int height = stoi(config["WINDOW_HEIGHT"]);

        log("The game resolution at start will be " + std::to_string(width) + "x" + std::to_string(height) + "! Selected from config file.");
        return { width, height };
    }

    error_log("The resolution in the config file (" + string_config_resolution + ") is not a 16/9 resolution! Switching to monitor resolution..");

    // We try to find the monitor resolution in the allowed resolutions list.
    if (find(allowed_game_resolutions.begin(), allowed_game_resolutions.end(), string_monitor_resolution) != allowed_game_resolutions.end())
    {
        // Retrieve the monitor resolution data.
        int width = monitor_resolution.first;
        int height = monitor_resolution.second;

        log("The game resolution at start will be " + std::to_string(width) + "x" + std::to_string(height) + "! Selected from monitor resolution.");
        return { width, height };
    }

    error_log("The monitor resolution (" + string_monitor_resolution + ") is not a 16/9 resolution! Switching to allowed game resolutions..");

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
