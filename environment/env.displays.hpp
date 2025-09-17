#include <vector>
#include <string>

#ifndef ENVIRONNEMENT_DISPLAYS_HPP
#define ENVIRONNEMENT_DISPLAYS_HPP

std::vector<int> get_available_display_indexes();

std::string get_display_name
(
    const int &display_index
);

#endif
