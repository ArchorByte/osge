#include <map>
#include <string>

#ifndef HELPER_PARSER_HPP
#define HELPER_PARSER_HPP

std::map<std::string, std::string> parse_config_file
(
    const std::string &path
);

#endif
