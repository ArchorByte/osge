#include "../utils.hpp"

#include <ctime>
#include <string>
#include <vector>

/*
    Get the timestamp of the actual time.

    Tasks:
        1) Get the timestamp.

    Parameters:
        No parameters.

    Returns:
        The current timestamp.
*/
time_t Miscellaneous::get_current_timestamp()
{
    const time_t timestamp = time(nullptr);
    return timestamp;
}



/*
    Get the date of a timestamp.

    Tasks:
        1) Get the date using the host timezone.
        2) Get all relevant information and fix them.

    Parameters:
        - timestamp / time_t / Timestamp used to get the date from.

    Returns:
        A vector list containing the date. Output format: year (YYYY), month (MM), day (DD), hour (HH), minutes (MM), seconds (SS).
*/
std::vector<std::string> Miscellaneous::get_date_from_timestamp
(
    const time_t &timestamp
)
{
    const struct tm date = *localtime(&timestamp);

    const std::string year = std::to_string(date.tm_year + 1900); // date.tm_year returns the amount of years that passed since 1900.
    const std::string month = std::to_string(date.tm_mon + 1);    // date.tm_mon returns the month number from 0 to 11.
    const std::string day = std::to_string(date.tm_mday);
    const std::string hour = std::to_string(date.tm_hour);
    const std::string minutes = std::to_string(date.tm_min);
    const std::string seconds = std::to_string(date.tm_sec);

    const std::vector<std::string> output =
    {
        year,
        month.size() < 2 ? "0" + month : month, // Add a 0 if necessary to make a 2 digits output.
        day.size() < 2 ? "0" + day : day,
        hour.size() < 2 ? "0" + hour : hour,
        minutes.size() < 2 ? "0" + minutes : minutes,
        seconds.size() < 2 ? "0" + seconds : seconds
    };

    return output;
}
