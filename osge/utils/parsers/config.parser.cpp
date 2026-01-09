#include "utils.parsers.hpp"

#include "../utils.hpp"

#include <filesystem>
#include <fstream>
#include <map>
#include <string>

/*
    Parse a .conf / .config file.
    Warning: If 2+ lines in the config file declare the same key, the most recent value found will be registered!

    Tasks:
        1) Verify the file path and extension.
        2) Verify the existence of the file.
        3) Open it and read it.
        4) For each line:
            - Check if the line is not empty or a comment.
            - Get the equal sign position.
            - Get the keys and values, and trim them.
            - Check that the key is not empty (we do tolerate empty values).
            - Register the data.

    Parameters:
        - file_path / string / Path to the config file to parse.

    Returns:
        A map containing the parsed keys and values.
*/
std::map<std::string, std::string> Parsers::parse_config_file
(
    const std::string &file_path
)
{
    Utils::Logs::log("Parsing the \"" + file_path + "\" config file..");

    std::ifstream file(file_path);
    std::map<std::string, std::string> output;
    std::string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.empty() || line[0] == '#')
                continue;

            const size_t equal_sign_position = line.find("=");

            if (equal_sign_position == std::string::npos)
            {
                Utils::Logs::error_log("Warning: Failed to read the malformed line: \"" + line + "\"!");
                continue;
            }

            const std::string key = Utils::Text::trim(line.substr(0, equal_sign_position));
            const std::string value = Utils::Text::trim(line.substr(equal_sign_position + 1));

            if (key.empty())
            {
                Utils::Logs::error_log("");
                continue;
            }

            output[key] = value;
        }
    }
    else Utils::Logs::error_log("Failed to open the following config file for parsing: \"" + file_path + "\"!");

    file.close();
    Utils::Logs::log("Config file (" + file_path + ") parsing finished! " + std::to_string(output.size()) + " keys have been found!");

    return output;
}
