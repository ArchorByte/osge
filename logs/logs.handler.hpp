#include <string>

#ifndef LOGS_HANDLER_HPP
#define LOGS_HANDLER_HPP

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
