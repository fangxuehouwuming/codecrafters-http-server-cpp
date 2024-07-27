
#include "include/config.hpp"
#include <iostream>

Config::Config(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--directory" && i + 1 < argc) {
            directory_ = argv[++i];
        }
    }
}

const std::string& Config::GetDirectory() const {
    return directory_;
}
