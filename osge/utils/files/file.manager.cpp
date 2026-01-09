#include "utils.files.hpp"

#include "../logs/utils.logs.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

/*
    Create a new empty file.

    Tasks:
        1) Verify the file path.
        2) Verify that another file doesn't already exist.
        3) Try to create the file.

    Parameters:
        - file_path / string / Targeted path + new file name.

    Returns:
        A boolean confirming whether we successfully created the empty file or not.
*/
bool Files::create_new_empty_file
(
    const std::string &file_path
)
{
    const bool has_filename = std::filesystem::path(file_path).has_filename();

    if (!has_filename)
    {
        Logs::error_log("Empty file creation failed! The path provided (\"" + file_path + "\") doesn't contain any file name!");
        return false;
    }

    const bool file_exists = std::filesystem::exists(file_path);

    if (file_exists)
    {
        Logs::error_log("Empty file creation failed! \"" + file_path + "\" already exists!");
        return false;
    }

    std::ofstream file(file_path);
    const bool file_opened = file.is_open();

    if (file_opened)
    {
        file.close();
        Logs::log("New empty file \"" + file_path + "\" created successfully!");
        return true;
    }

    Logs::error_log("Empty file creation failed! Failed to create \"" + file_path + "\"!");
    return false;
}



/*
    Write some data into a file.
    Warning: If append is false, the file will be entirely overwritten and all previous data will be lost!

    Tasks:
        1) Verify the file path.
        2) If the file doesn't exist, we try to create it.
        3) Open the file in the selected mode.
        4) Write data into the file.

    Parameters:
        - append    / bool   / Append the file or overwrite.
        - data      / string / Data to write into the file.
        - file_path / string / Path to the file we want to write data into.

    Returns:
        A boolean confirming whether we successfully wrote into the file or not.
*/
bool Files::write_file
(
    const bool &append,
    const std::string &data,
    const std::string &file_path
)
{
    std::ofstream file;
    const bool has_filename = std::filesystem::path(file_path).has_filename();

    if (!has_filename)
    {
        Logs::error_log("File writing failed! The path provided (\"" + file_path + "\") doesn't contain any file name!");
        return false;
    }

    const bool file_exists(std::filesystem::exists(file_path));

    if (!file_exists)
    {
        const bool file_created = create_new_empty_file(file_path);

        if (!file_created)
        {
            Logs::error_log("File writing failed! Failed to create \"" + file_path + "\" as it didn't already exist!");
            return false;
        }
    }

    if (append)
        file.open(file_path, std::ios::out | std::ios::app);   // Append mode.
    else
        file.open(file_path, std::ios::out | std::ios::trunc); // Overwrite mode.

    const bool is_opened = file.is_open();

    if (!is_opened)
    {
        Logs::error_log("File writing failed! Failed to open the following file for writing: \"" + file_path + "\"!");
        return false;
    }

    file << data;
    file.close();

    return true;
}
