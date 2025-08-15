#include <GLFW/glfw3.h>

#ifndef GLFW_UPDATER_HPP
#define GLFW_UPDATER_HPP

void update_glfw_window
(
    GLFWwindow* window,
    int width,
    int height,
    int refresh_rate,
    GLFWmonitor* monitor
);

#endif
