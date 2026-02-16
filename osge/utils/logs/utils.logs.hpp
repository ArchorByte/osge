#ifndef UTILS_LOGS_HPP
#define UTILS_LOGS_HPP

#include <string>

namespace Logs
{
    /////////////////////////
    ///// logs.file.cpp /////
    /////////////////////////

    void write_log_file
    (
        const std::string &log_type,
        const std::string &log
    );

    ////////////////////////////
    ///// logs.handler.cpp /////
    ////////////////////////////

    void log
    (
        const std::string &message,
        const bool        &end_line
    );

    void crash_log
    (
        const std::string &message
    );
}

#endif
