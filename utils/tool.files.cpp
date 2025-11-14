#include "tool.files.hpp"

#include "tool.text.format.hpp"
#include "../logs/logs.handler.hpp"

#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

// Return the content of a binary file.
std::vector<char> read_binary_file
(
    const std::string &file_path
)
{
    std::vector<char> output;

    if (trim(file_path).size() < 1)
    {
        error_log("Binary file reading failed! The path provided (\"" + file_path + "\") is not valid!");
        return output;
    }

    const bool has_filename = std::filesystem::path(file_path).has_filename();

    if (!has_filename)
    {
        error_log("Binary file reading failed! The path provided (\"" + file_path + "\") doesn't contain any file name!");
        return output;
    }

    const bool file_exists(std::filesystem::exists(file_path));

    if (!file_exists)
    {
        error_log("Binary file reading failed! The file \"" + file_path + "\" doesn't exist!");
        return output;
    }

    // Open the file in read-only binary mode.
    std::ifstream file (file_path, std::ios::ate | std::ios::binary);
    const bool is_opened = file.is_open();

    if (!is_opened)
    {
        error_log("Failed to open the following binary file for reading: \"" + file_path + "\"!");
        return output;
    }

    const size_t file_size = static_cast<size_t>(file.tellg());

    if (file_size < 1)
    {
        error_log("Failed to read the following binary file because it's empty: \"" + file_path + "\"!");
        return output;
    }

    output.resize(file_size);
    file.seekg(0); // Set our position to the first binary of the file.

    if (!file.read(output.data(), file_size))
    {
        error_log("Failed to read the following binary file: \"" + file_path + "\"!");
        output.clear();
    }

    file.close(); // Free the file.
    return output;
}

// Create a new empty file.
// Note: Return true on success and false on failure.
bool create_new_empty_file
(
    const std::string &file_path
)
{
    if (trim(file_path).size() < 1)
    {
        error_log("Empty file creation failed! The path provided (\"" + file_path + "\") is not valid!");
        return false;
    }

    const bool has_filename = std::filesystem::path(file_path).has_filename();

    if (!has_filename)
    {
        error_log("Empty file creation failed! The path provided (\"" + file_path + "\") doesn't contain any file name!");
        return false;
    }

    const bool file_exists = std::filesystem::exists(file_path);

    if (file_exists)
    {
        error_log("Empty file creation failed! The file (\"" + file_path + "\") already exists!");
        return false;
    }

    // Try to create the file.
    std::ofstream file(file_path);
    const bool file_opened = file.is_open();

    if (file_opened)
    {
        file.close();
        log("File \"" + file_path + "\" created successfully!");
        return true;
    }

    error_log("Failed to create the \"" + file_path + "\" file!");
    return false;
}

// Write data into a file.
// Note: Outside of the append mode, the file will be completely overwritten.
bool write_file
(
    const std::string &file_path,
    const std::string &input,
    const bool &append
)
{
    std::ofstream file;

    if (trim(file_path).size() < 1)
    {
        error_log("File writing failed! The path provided (\"" + file_path + "\") is not valid!");
        return false;
    }

    const bool has_filename = std::filesystem::path(file_path).has_filename();

    if (!has_filename)
    {
        error_log("File writing failed! The path provided (\"" + file_path + "\") doesn't contain any file name!");
        return false;
    }

    const bool file_exists(std::filesystem::exists(file_path));

    if (!file_exists)
    {
        // Try to create the targeted file.
        bool file_creation = create_new_empty_file(file_path);

        if (!file_creation)
        {
            error_log("File writing data into \"" + file_path + "\" failed! Failed to create the file \"" + file_path + "\"!");
            return false;
        }
    }

    if (append) file.open(file_path, std::ios::out | std::ios::app); // Open the file in append mode.
    else file.open(file_path, std::ios::out | std::ios::trunc);      // Open the file in overwrite mode.

    const bool is_opened = file.is_open();

    if (!is_opened)
    {
        error_log("Failed to open the \"" + file_path + "\" file!");
        return false;
    }

    // Try to write the data into the file.
    file << input;

    if (!file)
    {
        error_log("Failed to write data into the \"" + file_path + "\" file!");
        return false;
    }

    file.close(); // Free the file.
    return true;
}
