#include "file.hpp"

#include "../tools/files.hpp"
#include "../tools/text_format.hpp"
#include "terminal.hpp"
#include "../tools/time_manager.hpp"

#include <string>
#include <ctime>

// Write the logs in the logs file.
void write_log_file
(
    const std::string &log_type, // Log, error, or fatal error by default.
    const std::string &log
)
{
    if (trim(log_type).size() < 1)
    {
        error_log("Log writing failed! The log type provided is empty!");
        return;
    }

    if (trim(log).size() < 1)
    {
        error_log("Log writing failed! The log provided is empty!");
        return;
    }

    const time_t timestamp = get_current_timestamp();
    const std::vector<std::string> date = get_date_from_timestamp(timestamp);

    // Date format: MM/DD/YY-HH/MN/SS.
    const std::string date_format = date[1] + "/" + date[2] + "/" + date[0] + "-" + date[3] + ":" + date[4] + ":" + date[5];

    // Log format: DATE [LOG_TYPE] LOG.
    const std::string log_format = date_format + " [" + log_type + "] " + log;

    write_file("osge.logs", log_format, true, true);
}
