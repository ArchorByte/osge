#include "../utils.hpp"

#include <ctime>
#include <string>
#include <vector>

/*
    Write some data into the game logs file.

    Tasks:
        1) Verify the parameters.
        2) Get the current date.
        3) Format the output log.
        4) Write the formatted output into the file.

    Parameters:
        - log_type / string / Type of the log message. By default, the engine gives "log", "error" and "crash".
        - log      / string / Log message.
*/
void Logs::write_log_file
(
    const std::string &log_type,
    const std::string &log
)
{
    if (Utils::Text::trim(log_type).size() < 1)
    {
        Logs::error_log("Log file writing failed! The log type provided is empty!");
        return;
    }

    if (Utils::Text::trim(log).size() < 1)
    {
        Logs::error_log("Log file writing failed! The log provided is empty!");
        return;
    }

    const time_t timestamp = Utils::Miscellaneous::get_current_timestamp();
    const std::vector<std::string> date = Utils::Miscellaneous::get_date_from_timestamp(timestamp);

    // Date format: MM/DD/YY-HH/MN/SS.
    const std::string date_format = date[1] + "/" + date[2] + "/" + date[0] + "-" + date[3] + ":" + date[4] + ":" + date[5];

    // Log format: DATE [LOG_TYPE] LOG.
    const std::string log_format = date_format + " [" + log_type + "] " + log + "\n";

    Utils::Files::write_file(true, "osge.logs", log_format);
}
