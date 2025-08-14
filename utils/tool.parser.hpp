#include <map>
#include <string>

#ifndef TOOLS_PARSER_HPP
#define TOOLS_PARSER_HPP

std::map<std::string, std::string> parse_config_file
(
    const std::string &path
);

#endif
