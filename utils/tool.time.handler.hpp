#include <ctime>
#include <vector>
#include <string>

#ifndef TOOLS_TIME_HPP
#define TOOLS_TIME_HPP

time_t get_current_timestamp();

std::vector<std::string> get_date_from_timestamp
(
    const time_t &timestamp
);

#endif
