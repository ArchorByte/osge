#include "terminal.hpp"

#include "../config.hpp"
#include "file.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

// Sends an info log only if debug mode enabled.
// Writes an info log in the logs file if the logs file is allowed.
void log
(
    const std::string &log
)
{
    #ifdef DEBUG_MODE
        std::cout << "[log] " << log << std::endl;
    #endif

    #ifdef ALLOW_LOGS_FILE
        write_log_file("log", log);
    #endif
}

// Sends an error log if debug mode enabled.
// Writes an error log in the logs file if the logs file is allowed.
void error_log
(
    const std::string &log
)
{
    #ifdef DEBUG_MODE
        std::cerr << "[error] " << log << std::endl;
    #endif

    #ifdef ALLOW_LOGS_FILE
        write_log_file("error", log);
    #endif
}

// Sends a fatal info log if debug mode enabled.
// Writes an error log in the logs file if the logs file is allowed.
void fatal_error_log
(
    const std::string &log
)
{
    #ifdef DEBUG_MODE
        std::cerr << "[fatal_error] " << log << std::endl;
    #endif

    #ifdef ALLOW_LOGS_FILE
        write_log_file("fatal_error", log);
    #endif

    throw std::runtime_error(log); // Trigger the crash handling process.
}
