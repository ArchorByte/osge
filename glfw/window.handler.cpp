#include "window.handler.hpp"

#include "../logs/logs.handler.hpp"
#include "../config/engine.config.hpp"
#include "../utils/tool.text.format.hpp"

#include <GLFW/glfw3.h>
#include <string>
#include <map>
#include <iostream>

//////////////////////////////////////////////////////
//////////////////// ENUMERATIONS ////////////////////
//////////////////////////////////////////////////////

// Note: any number not explicitly handled will be defaulted to FULLSCREEN mode.
enum Window_Modes
{
    WINDOWED = 0,
    BORDERLESS = 1,
    FULLSCREEN = 2
};

// Note: any number not explicitly handled will be defaulted to VULKAN.
enum GraphicsAPI
{
    VULKAN = 0,
    OPENGL = 1
};

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a window using GLFW.
// Warning: We assume that GLFW has already been initialized! Normally, the engine already takes care of that on startup.
GLFWwindow* create_glfw_window
(
    int width,
    int height,
    int window_mode,
    std::string window_name,
    GLFWmonitor* monitor,
    int graphic_api
)
{
    log("Creating a GLFW window..");

    if (!monitor)
    {
        fatal_error_log("GLFW window creation failed! The monitor provided (" + force_string(monitor) + ") is not valid!");
    }

    if (window_mode < WINDOWED || window_mode > FULLSCREEN)
    {
        error_log("Warning: Invalid window mode provided! Defaulted to full screen mode.");
        window_mode = FULLSCREEN; // Defaulted to full screen mode.
    }

    if (graphic_api < VULKAN || graphic_api > OPENGL)
    {
        error_log("Warning: Invalid graphic API provided! Defaulted to Vulkan.");
        graphic_api = VULKAN; // Defaulted to Vulkan.
    }

    if (!glfwVulkanSupported() && graphic_api == VULKAN)
    {
        fatal_error_log("GLFW window creation failed! Missing Vulkan support for GLFW on this device!");
    }

    if (graphic_api != OPENGL) glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Disable OpenGL tools if we are not running OpenGL.
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Deny any external window resize.
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);   // Hide the window for now.

    // We need to do some extra steps in full screen mode.
    if (window_mode == FULLSCREEN)
    {
        const GLFWvidmode* video_mode = glfwGetVideoMode(monitor);

        if (!video_mode)
        {
            fatal_error_log("GLFW window creation failed! Failed to get the monitor video mode!");
        }

        // We are obliged to overwrite the width and height inputs to have a full screen window.
        // However, it doesn't affect the rendering resolution.
        width = video_mode -> width;
        height = video_mode -> height;
    }

    // We disable the window decorations in borderless mode.
    if (window_mode == BORDERLESS)
    {
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    }

    // We modify the window name in debug mode.
    #ifdef DEBUG_MODE
        window_name += " - Debug Mode";
    #endif

    GLFWwindow* window = glfwCreateWindow(width, height, window_name.c_str(), monitor, nullptr);

    if (!window)
    {
        fatal_error_log("The GLFW window creation failed!");
    }

    glfwShowWindow(window); // Ensure that the window is shown once we finished its creation.
    log("GLFW window " + force_string(window) + " created successfully!");

    return window;
}

// Destroy a GLFW window.
void destroy_window
(
    GLFWwindow* window
)
{
    log("Destroying the " + force_string(window) + " GLFW window..");

    if (!window)
    {
        error_log("GLFW window destruction failed! The window provided (" + force_string(window) + ") is not valid!");
        return;
    }

    glfwDestroyWindow(window);
    log("GLFW window destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
GLFW_Window::GLFW_Window
(
    int width,
    int height,
    int window_mode,
    std::string window_name,
    GLFWmonitor* monitor,
    int graphic_api
)
{
    window = create_glfw_window(width, height, window_mode, window_name, monitor, graphic_api);
}

// Destructor.
GLFW_Window::~GLFW_Window()
{
    destroy_window(window);
}

GLFWwindow* GLFW_Window::get() const
{
    return window;
}
