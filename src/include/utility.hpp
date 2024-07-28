#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include <vector>

std::vector<std::string> SplitMessage(const std::string& message, const std::string& delim);
std::string CompressGzip(const std::string& str);
#endif  // UTILITY_HPP
