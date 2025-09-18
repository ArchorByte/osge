#include "help.parser.hpp"

#include "help.text.format.hpp"
#include "../logs/logs.handler.hpp"

#include <fstream>
#include <string>
#include <map>

// Return a map of the config file data.
std::map<std::string, std::string> parse_config_file
(
    const std::string &file_path
)
{
    log("Parsing the \"" + file_path + "\" config file..");
    std::ifstream file(file_path);

    std::map<std::string, std::string> output;
    std::string line;

    if (file.is_open())
    {
        while (getline(file, line)) // Read each line.
        {
            // We ignore the empty lines and comments.
            if (line.empty() || line[0] == '#') continue;

            // Find the equal sign position in the line.
            const size_t equal_sign_position = line.find("=");

            if (equal_sign_position == std::string::npos)
            {
                error_log("Warning: Failed to read the malformed line: \"" + line + "\"!");
                continue;
            }

            const std::string key = line.substr(0, equal_sign_position);    // Take everything before the equal sign.
            const std::string value = line.substr(equal_sign_position + 1); // Same but after.

            // Warning: If two lines declare the same value, the last one will overwrite the most recent one.
            output[trim(key)] = trim(value);
        }
    }
    else error_log("Failed to open the following config file for parsing: \"" + file_path + "\"!");

    file.close(); // Free the file.
    log("Config file (\"" + file_path + "\") parsing finished! " + std::to_string(output.size()) + " keys have been found!");
    return output;
}
