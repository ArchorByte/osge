#include "../utils.hpp"

#include "../config/engine.config.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

/*
    Send a message in the terminal (if enabled) and write it into the logs file (if enabled).

    Tasks:
        1) Verify the engine configuration.
        2) Depending on it, we either send a message in the terminal, or write it into the logs file, or both.

    Parameters:
        - message / string / Message to log.

    Returns:
        No object returned.
*/
void Logs::log
(
    const std::string &message
)
{
    if constexpr (EngineConfig::DEBUG_MODE)
        std::cout << "[log] " << message << "\n";

    if constexpr (EngineConfig::ENABLE_LOGS_FILE)
        Logs::write_log_file("log", message);
}



/*
    Send a message as an error in the terminal (if enabled) and write it into the logs file (if enabled).

    Tasks:
        1) Verify the engine configuration.
        2) Depending on it, we either send a message in the terminal, or write it into the logs file, or both.

    Parameters:
        - message / string / Message to log as an error.

    Returns:
        No object returned.
*/
void Logs::error_log
(
    const std::string &message
)
{
    if constexpr (EngineConfig::DEBUG_MODE)
        std::cout << "[error] " << message << "\n";

    if constexpr (EngineConfig::ENABLE_LOGS_FILE)
        Logs::write_log_file("error", message);
}



/*
    Send a message as a crash in the terminal (if enabled) and write it into the logs file (if enabled).

    Tasks:
        1) Verify the engine configuration.
        2) Depending on it, we either send a message in the terminal, or write it into the logs file, or both.
        3) Trigger the crash handler with a runtime error.

    Parameters:
        - message / string / Message to log as a crash.

    Returns:
        No object returned.
*/
void Logs::crash_error_log
(
    const std::string &message
)
{
    if constexpr (EngineConfig::DEBUG_MODE)
        std::cout << "[crash] " << message << "\n";

    if constexpr (EngineConfig::ENABLE_LOGS_FILE)
        Logs::write_log_file("crash", message);

    throw std::runtime_error(message);
}
