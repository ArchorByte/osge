#include <vector>
#include <GLFW/glfw3.h>
#include <string>

#ifndef ENVIRONNEMENT_DISPLAYS_HPP
#define ENVIRONNEMENT_DISPLAYS_HPP

std::vector<int> get_available_displays();

std::string get_display_name
(
    int display_index
);

#endif
