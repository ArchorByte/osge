#include "monitors.hpp"

#include "../logs/terminal.hpp"
#include "../tools/text_format.hpp"

#include <GLFW/glfw3.h>
#include <vector>
#include <string>

// Retrieve and list all available monitors.
// Warning: We assume that GLFW has already been initialized! Normally, the engine takes care of that by default on startup.
std::vector<GLFWmonitor*> get_available_monitors()
{
    log("Retrieving all available monitors on this device..");
    std::vector<GLFWmonitor*> output;

    int monitors_count = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&monitors_count);

    if (!monitors || monitors_count < 1)
    {
        fatal_error_log("No monitors were found on this device!");
        return output; // Useless as fatal_error_log exits the app, but called anyway to avoid unnecessary compiler warnings.
    }

    // Register all monitors in the list.
    for (int i = 0; i < monitors_count; i++)
    {
        output.emplace_back(monitors[i]);
    }

    log(std::to_string(output.size()) + " monitors were found!");
    return output;
}

// Retrieve the name of a monitor.
std::string get_monitor_name
(
    GLFWmonitor* monitor
)
{
    if (!monitor)
    {
        error_log("Invalid monitor provided (" + force_string(monitor) + ")! Returned \"Unknown Monitor\" by default.");
        return "Unknown Monitor";
    }

    const char* monitor_name = glfwGetMonitorName(monitor);
    std::string output = std::string(monitor_name);

    return output;
}
