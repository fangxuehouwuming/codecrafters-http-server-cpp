#include "include/response.hpp"
#include <netdb.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "include/utility.hpp"

Response Response::GenerateResponse(const Request& request, const Config& server_config) {
    Response response;
    response.status_line_ = "HTTP/1.1 200 OK\r\n";
    response.headers_["Content-Type"] = "text/plain\r\n";
    response.body_ = "";

    std::string path = request.GetPath();
    std::vector<std::string> spilt_path = SplitMessage(path, "/");

    if (request.GetMethod() == "GET") {
        if (path == "/") {
            response.response_ = "HTTP/1.1 200 OK\r\n\r\n";
            return response;
        } else if (spilt_path[1] == "echo") {
            response.body_ = spilt_path[2];
        } else if (spilt_path[1] == "user-agent") {
            response.body_ = request.GetHeaders().at("User-Agent");
        } else if (spilt_path[1] == "files") {
            std::string filename = spilt_path[2];
            std::string server_files_dir = server_config.GetDirectory();
            std::string server_file_path = server_files_dir + filename;
            std::ifstream file(server_file_path);
            if (file.is_open()) {
                std::stringstream buffer;
                buffer << file.rdbuf();
                response.headers_["Content-Type"] = "application/octet-stream\r\n";
                response.body_ = buffer.str();
            } else {
                response.response_ = "HTTP/1.1 404 Not Found\r\n\r\n";
                return response;
            }
        } else {
            response.response_ = "HTTP/1.1 404 Not Found\r\n\r\n";
            return response;
        }

        if (request.GetHeaders().count("Accept-Encoding")) {
            response.body_ = Gzip::Compress(response.body_);
            auto accept_encoding_list = SplitMessage(request.GetHeaders().at("Accept-Encoding"), ", ");
            for (const auto& accept_encoding : accept_encoding_list) {
                if (accept_encoding == "gzip") {
                    response.body_ = CompressGzip(response.body_);
                    response.headers_["Content-Encoding"] = "gzip\r\n";
                    break;
                }
            }
        }

        response.headers_["Content-Length"] = std::to_string(response.body_.size()) + "\r\n";
        response.response_ = response.ComposeResponse();

    } else if (request.GetMethod() == "POST") {
        if (spilt_path[1] == "files") {
            std::string file_name = spilt_path[2];
            std::string file_path = server_config.GetDirectory() + file_name;
            std::ofstream file(file_path);
            file << request.GetBody();
            file.close();
            response.response_ = "HTTP/1.1 201 Created\r\n\r\n";
        }
    }
    return response;
}

void Response::Send(int client_fd) const {
    send(client_fd, response_.c_str(), response_.size(), 0);
}

std::string Response::ComposeResponse() const {
    std::string composed_response = status_line_;
    for (const auto& header : headers_) {
        composed_response += header.first + ": " + header.second;
    }
    composed_response += "\r\n" + body_;
    return composed_response;
}