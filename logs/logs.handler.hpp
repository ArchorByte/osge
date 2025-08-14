#include <string>

#ifndef LOGS_TERMINAL_HPP
#define LOGS_TERMINAL_HPP

void log
(
    const std::string &log
);

void error_log
(
    const std::string &log
);

void fatal_error_log
(
    const std::string &log
);

#endif
