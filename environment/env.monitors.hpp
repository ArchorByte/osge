#include <vector>
#include <GLFW/glfw3.h>
#include <string>

#ifndef ENVIRONNEMENT_MONITORS_HPP
#define ENVIRONNEMENT_MONITORS_HPP

std::vector<GLFWmonitor*> get_available_monitors();

std::string get_monitor_name
(
    GLFWmonitor* monitor
);

#endif
