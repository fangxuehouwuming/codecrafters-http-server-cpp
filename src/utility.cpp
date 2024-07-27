#include "include/utility.hpp"

std::vector<std::string> SplitMessage(const std::string& message, const std::string& delim) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    while ((end = message.find(delim, start)) != std::string::npos) {
        tokens.push_back(message.substr(start, end - start));
        start = end + delim.size();
    }
    tokens.push_back(message.substr(start));
    return tokens;
}
