#include "include/request.hpp"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include "include/utility.hpp"

namespace {
const int kBufferSize = 1024;
}

bool Request::Parse(int client_fd) {
    char buffer[kBufferSize] = {0};
    int valread = read(client_fd, buffer, kBufferSize);
    if (valread < 0) {
        std::cerr << "Read failed\n";
        return false;
    }

    std::string request(buffer);
    std::cout << "Received request: " << request << std::endl;

    std::vector<std::string> lines = SplitMessage(request, "\r\n");
    if (lines.empty()) {
        return false;
    }

    std::vector<std::string> request_line = SplitMessage(lines[0], " ");
    if (request_line.size() < 3) {
        return false;
    }

    method_ = request_line[0];
    path_ = request_line[1];
    version_ = request_line[2];

    size_t i = 1;
    for (; i < lines.size(); ++i) {
        if (lines[i].empty()) {
            break;
        }
        size_t pos = lines[i].find(": ");
        if (pos != std::string::npos) {
            std::string key = lines[i].substr(0, pos);
            std::string value = lines[i].substr(pos + 2);
            headers_[key] = value;
        }
    }

    if (i < lines.size() - 1) {
        body_ = lines[i + 1];
    }

    return true;
}

const std::string& Request::GetMethod() const {
    return method_;
}

const std::string& Request::GetPath() const {
    return path_;
}

const std::string& Request::GetVersion() const {
    return version_;
}

const std::unordered_map<std::string, std::string>& Request::GetHeaders() const {
    return headers_;
}

const std::string& Request::GetBody() const {
    return body_;
}