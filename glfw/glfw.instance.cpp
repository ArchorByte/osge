#include "glfw.instance.hpp"
#include "../logs/logs.handler.hpp"

#include <GLFW/glfw3.h>

// Try to initialize GLFW.
void start_glfw_instance()
{
    log("Initializing GLFW..");
    bool initialization = glfwInit();

    if (!initialization)
    {
        fatal_error_log("Failed to initialize GLFW!");
    }

    log("GLFW successfully initialized!");
}

// Destroy the GLFW instance.
void destroy_glfw_instance()
{
    log("Destroying the GLFW instance..");
    glfwTerminate();
    log("GLFW instance destroyed successfully!");
}
