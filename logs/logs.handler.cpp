#include "logs.handler.hpp"

#include "logs.file.hpp"
#include "../config/engine.config.hpp"

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
    if constexpr (EngineConfig::DEBUG_MODE)
    {
        std::cout << "[log] " << log << "\n";
    }

    if constexpr (EngineConfig::ENABLE_LOGS_FILE)
        write_log_file("log", log);
}

// Sends an error log if debug mode enabled.
// Writes an error log in the logs file if the logs file is allowed.
void error_log
(
    const std::string &log
)
{
    if constexpr (EngineConfig::DEBUG_MODE)
    {
        std::cout << "[error] " << log << "\n";
    }

    if constexpr (EngineConfig::ENABLE_LOGS_FILE)
        write_log_file("error", log);
}

// Sends a fatal info log if debug mode enabled.
// Writes an error log in the logs file if the logs file is allowed.
void fatal_error_log
(
    const std::string &log
)
{
    if constexpr (EngineConfig::DEBUG_MODE)
    {
        std::cout << "[fatal_error] " << log << "\n";
    }

    if constexpr (EngineConfig::ENABLE_LOGS_FILE)
        write_log_file("fatal_error", log);

    throw std::runtime_error(log); // Trigger the crash handling process.
}
