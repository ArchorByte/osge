#include "utils.files.hpp"

#include "../utils.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

/*
    Open a file in binary mode and get the binaries.
    Warning: On fail, we return an empty vector list!

    Tasks:
        1) Verify the file path.
        2) Verify the file existence.
        3) Try to open the file.
        4) Get the binaries and put them in the output vector list.

    Parameters:
        - file_path / string / Path to the targeted file.

    Returns:
        A vector list containing the binaries.
*/
std::vector<char> Files::read_binary_file
(
    const std::string &file_path
)
{
    std::vector<char> output;
    const bool has_filename = std::filesystem::path(file_path).has_filename();

    if (!has_filename)
    {
        Logs::error_log("Binary file reading failed! The path provided (\"" + file_path + "\") doesn't contain any file name!");
        return output;
    }

    const bool file_exists(std::filesystem::exists(file_path));

    if (!file_exists)
    {
        Logs::error_log("Binary file reading failed! No such file or directory!");
        return output;
    }

    std::ifstream file (file_path, std::ios::ate | std::ios::binary); // Read-only binary mode.
    const bool is_opened = file.is_open();

    if (!is_opened)
    {
        Logs::error_log("Binary file reading failed! Failed to open the following file for reading: \"" + file_path + "\"!");
        return output;
    }

    const size_t file_size = static_cast<size_t>(file.tellg());

    if (file_size < 1)
    {
        Logs::error_log("Binary file reading failed! Failed to read the following file as it's empty: \"" + file_path + "\"!");
        return output;
    }

    output.resize(file_size);
    file.seekg(0); // Set position to first binary.

    if (!file.read(output.data(), file_size))
    {
        Logs::error_log("Binary file reading failed! Failed to read the following file: \"" + file_path + "\"!");
        output.clear();
    }

    file.close();
    return output;
}
