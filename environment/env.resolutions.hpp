#include <vector>
#include <string>
#include <map>

#ifndef ENVIRONMENT_RESOLUTIONS_HPP
#define ENVIRONMENT_RESOLUTIONS_HPP

std::pair<int, int> get_display_resolution
(
    const int &display_index
);

std::vector<std::string> get_allowed_game_resolutions
(
    const std::pair<int, int> &monitor_resolution
);

std::pair<int, int> select_game_resolution
(
    std::map<std::string, std::string> config,
    const std::pair<int, int> &screen_resolution,
    const std::vector<std::string> &allowed_game_resolutions
);

#endif
