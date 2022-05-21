#pragma once
#include <string.h>
#include <regex>
#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>

inline bool replace(std::string& str, std::string from, std::string to) {
    if(str.find(from) == std::string::npos)
        return false;
    str.replace(str.find(from), from.length(), to);
    return true;
}
