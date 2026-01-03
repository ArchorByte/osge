#include "utils.logs.hpp"

#include "../files/utils.files.hpp"
#include "../text/utils.text.hpp"
#include "../miscellaneous/utils.miscellaneous.hpp"

#include <ctime>
#include <string>

// Write data into the logs file.
void write_log_file
(
    const std::string &log_type,
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
    const std::string log_format = date_format + " [" + log_type + "] " + log + "\n";

    write_file("osge.logs", log_format, true);
}
