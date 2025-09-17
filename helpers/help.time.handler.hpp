#include <ctime>
#include <vector>
#include <string>

#ifndef HELPER_TIME_HANDLER_HPP
#define HELPER_TIME_HANDLER_HPP

time_t get_current_timestamp();

std::vector<std::string> get_date_from_timestamp
(
    const time_t &timestamp
);

#endif
