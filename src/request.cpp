#include "include/request.hpp"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include "include/utility.hpp"

namespace {
const int kBufferSize = 1024;
}

bool Request::Parse(int client_fd) {
    // request = "GET /echo/abc HTTP/1.1\r\nHost: localhost:4221\r\nUser-Agent: curl/7.64.1\r\nAccept: */*\r\n\r\n"
    // GET /echo/abc HTTP/1.1
    // Host: localhost:4221
    // User-Agent: curl/7.64.1
    // Accept: */*
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
    if (request_line.size() < 2) {
        return false;
    }

    method_ = request_line[0];
    path_ = request_line[1];

    for (const std::string& line : lines) {
        if (line.find("User-Agent:") == 0) {  // if line starts with "User-Agent:"
            std::vector<std::string> user_agent_parts = SplitMessage(line, " ");
            if (user_agent_parts.size() > 1) {
                user_agent_ = user_agent_parts[1];
            }
        }
    }

    return true;
}

const std::string& Request::GetMethod() const {
    return method_;
}

const std::string& Request::GetPath() const {
    return path_;
}

const std::string& Request::GetUserAgent() const {
    return user_agent_;
}