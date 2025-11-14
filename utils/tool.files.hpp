#include <vector>
#include <string>

#ifndef HELPER_FILES_HPP
#define HELPER_FILES_HPP

std::vector<char> read_binary_file
(
    const std::string &file_path
);

bool create_new_empty_file
(
    const std::string &file_path
);

bool write_file
(
    const std::string &file_path,
    const std::string &input,
    const bool &append
);

#endif
