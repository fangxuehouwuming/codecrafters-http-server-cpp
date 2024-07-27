#include "include/response.hpp"
#include <netdb.h>
#include <unistd.h>
#include "include/utility.hpp"

Response Response::GenerateResponse(const Request& request) {
    Response response;
    response.status_line_ = "HTTP/1.1 200 OK\r\n";
    response.content_type_ = "Content-Type: text/plain\r\n";
    response.content_length_ = 0;
    response.body_ = "";

    std::string path = request.GetPath();
    std::vector<std::string> spilt_path = SplitMessage(path, "/");

    if (path == "/") {
        response.response_ = "HTTP/1.1 200 OK\r\n\r\n";
        return response;

    } else if (spilt_path[1] == "echo") {
        response.body_ = spilt_path[2];
        response.content_length_ = response.body_.size();

    } else if (spilt_path[1] == "user-agent") {
        response.body_ = request.GetUserAgent();
        response.content_length_ = response.body_.size();

    } else {
        response.response_ = "HTTP/1.1 404 Not Found\r\n\r\n";
        return response;
    }

    response.response_ = response.status_line_ + response.content_type_ +
                         "Content-Length: " + std::to_string(response.content_length_) + "\r\n\r\n" +
                         response.body_;

    return response;
}

void Response::Send(int client_fd) const {
    send(client_fd, response_.c_str(), response_.size(), 0);
}
