#include "help.time.handler.hpp"

#include <ctime>
#include <string>
#include <vector>

// Return the timestamp of the current time.
time_t get_current_timestamp()
{
    time_t timestamp = time(nullptr);
    return timestamp;
}

// Return a date using a timestamp.
// Output format: year (YYYY), month (MM), day (DD), hour (HH), minutes (MM), seconds (SS).
std::vector<std::string> get_date_from_timestamp
(
    const time_t &timestamp
)
{
    // Get the date using the host time zone.
    struct tm date = *localtime(&timestamp);

    std::string year = std::to_string(date.tm_year + 1900); // date.tm_year returns the amount of years that passed since 1900, so we add 1900 to get the current year.
    std::string month = std::to_string(date.tm_mon + 1);    // date.tm_mon returns the month number from 0 to 11 instead of 1 to 12, so we add 1 to fix this "issue".
    std::string day = std::to_string(date.tm_mday);
    std::string hour = std::to_string(date.tm_hour);
    std::string minutes = std::to_string(date.tm_min);
    std::string seconds = std::to_string(date.tm_sec);

    // We add an extra zero to the output with only one character.
    // Examples: 1 -> 01 ; 5 -> 05 ; 8 -> 08.
    month = month.size() < 2 ? "0" + month : month;
    day = day.size() < 2 ? "0" + day : day;
    hour = hour.size() < 2 ? "0" + hour : hour;
    minutes = minutes.size() < 2 ? "0" + minutes : minutes;
    seconds = seconds.size() < 2 ? "0" + seconds : seconds;

    std::vector<std::string> output =
    {
        year,
        month,
        day,
        hour,
        minutes,
        seconds
    };

    return output;
}
