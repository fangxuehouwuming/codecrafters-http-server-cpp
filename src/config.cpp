
#include "include/config.hpp"
#include <iostream>

Config::Config(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--directory" && i + 1 < argc) {
            directory_ = argv[++i];
        }
    }
}

std::string Config::GetDirectory() {
    return directory_;
}
